"""
Frame times for the ICI 9320P.

The SDK stamps each frame when its last bytes arrive over USB (time.monotonic(), in the libusb
callback). That arrival is late by the USB transfer and by however long the callback waited for
Python (the GIL: up to a frame or more while another thread is busy). The sensor, though, ends a
frame every FRAME_PERIOD_S on its own crystal, so its true frame ends lie on a straight line,

    end(k) = phase + period * k        k = the sensor's frame number,

and each arrival is that plus a delay that is never negative. A line fitted *under* the arrivals
(least-squares slope; the offset at a low quantile of the residuals, i.e. through the least
delayed frames) recovers end(k) to well under a millisecond.

The frame numbers count the frames as they come: each sensor frame ends one transfer, so they
arrive in order, late perhaps, but none extra and normally none missing (the SDK keeps several
transfers queued, so a frame is only lost when Python doesn't read for longer than they last).
A loss shows as a step: the least delay over the next LOSS_WINDOW frames is a period or more
above the least delay over the previous LOSS_WINDOW (counted in order, against the nominal
period; comparing neighbours, a constant offset and the clocks' slow drift cancel out). A stall
doesn't: the frames that waited it out arrive together, in a burst, and the next ones are on
time again. A burst is the queued frames, from before any loss, so a loss is placed after a
burst, never inside one; and since no frame arrives before it ended, at the first frame from
which none would have (a frame just before a loss may itself have been late).

FrameClock does this live, over the last WINDOW frames, as each frame is read (it can't revise
frames already returned, so the frames read just after a loss are off until it is seen).
retime() does it afterwards over a whole recording.
"""
from collections import deque

import numpy as np

from . import protocol as P

WINDOW = 600            # frames (10 s) the live fit uses
MIN_FIT = 30            # frames before the live fit is used (until then: arrival times)
SEGMENT_GAP = 1.0       # [s] a pause this long (the stream stopped) starts a new fit
ENVELOPE = 0.02         # the quantile of the residuals the line passes through
LOSS_WINDOW = 30        # frames that must all be a period behind the line to count a loss
LOSS_THRESHOLD = 0.5    # [periods] "behind the line" for that
BURST = 0.3             # [periods] frames this close together arrived in a burst
EARLY = 0.1             # [periods] how far below the line jitter may put a frame


def _fit(k, a, period):
    """Return (phase, period) of the line under arrivals a of frames k."""
    k = np.asarray(k, float)
    a = np.asarray(a, float)
    if len(k) >= 2 and k[-1] > k[0]:
        kc = k - k.mean()
        period = float((kc * (a - a.mean())).sum() / (kc * kc).sum())
    phase = float(np.quantile(a - period * k, ENVELOPE))
    return phase, period


def _losses(a, period):
    """Return each frame's sensor frame number, counting the frames lost before it.

    a: the arrivals of every frame the SDK delivered, in order. See the module's help.
    """
    n = len(a)
    r = (a - a[0]) / period - np.arange(n)      # each frame's delay [periods], counted in order
    gap = np.diff(a, prepend=-np.inf) / period
    lost = np.zeros(n, np.int64)                # frames lost before each frame
    i = 1
    while i < n - LOSS_WINDOW + 1:
        lost[i] = lost[i - 1]
        if gap[i] >= BURST:
            back = slice(max(0, i - LOSS_WINDOW), i)
            before = (r[back] - lost[back]).min()
            step = int(np.floor(r[i:i + LOSS_WINDOW].min() - lost[i] - before + 1
                                - LOSS_THRESHOLD))
            if step >= 1:
                j = _place(r - lost[i] - before, gap, i, step)
                lost[i:j] = lost[i]
                lost[j] = lost[i] + step
                i = j
        i += 1
    if n > LOSS_WINDOW:
        lost[n - LOSS_WINDOW + 1:] = lost[n - LOSS_WINDOW]
    return np.arange(n) + lost


def _place(r, gap, i, step):
    """Return where a loss of step frames found at frame i goes.

    The first frame from i on (in LOSS_WINDOW) that isn't in a burst and wouldn't be early
    with the loss before it: r its delay [periods] against the line before the loss.
    """
    for j in range(i, min(len(r), i + LOSS_WINDOW)):
        if gap[j] >= BURST and r[j] - step >= -EARLY:
            return j
    return i


def retime(arrivals, period=P.FRAME_PERIOD_S):
    """Return the frames' true end-of-readout times and sensor frame numbers.

    Args:
        arrivals: Each frame's arrival time (Frame.arrival), every frame the SDK delivered, in
            order (whole or not: each was a sensor frame). The numbers count lost frames.
        period: The nominal frame period [s] (fitted from the arrivals).

    Returns:
        (times, index): numpy arrays; times on the arrivals' clock, index counting from 0 at the
        first frame. After a pause of SEGMENT_GAP or more (the stream stopped) each stretch is
        fitted on its own, and the numbering carries on across the pause by its length.
    """
    a = np.asarray(arrivals, float)
    times = np.empty(len(a))
    index = np.empty(len(a), np.int64)
    cuts = [0] + list(np.nonzero(np.diff(a) >= SEGMENT_GAP)[0] + 1) + [len(a)]
    nxt, per = 0, period
    for s, e in zip(cuts[:-1], cuts[1:]):
        seg = a[s:e]
        if s:
            nxt = int(index[s - 1] + max(1, round((seg[0] - times[s - 1]) / per)))
        k = _losses(seg, period)
        phase, per = _fit(k, seg, period)
        times[s:e] = phase + per * k
        index[s:e] = k + nxt
    return times, index


class FrameClock:
    """Each frame's true end of readout and sensor frame number, live (see the module's help)."""

    def __init__(self, period=P.FRAME_PERIOD_S, window=WINDOW):
        """Start with no frames."""
        self.nominal, self.window = period, window
        self.reset()

    def reset(self):
        """Forget every frame (the stream restarted)."""
        self._k = deque(maxlen=self.window)
        self._a = deque(maxlen=self.window)
        self.index = -1
        self.period = self.nominal
        self.phase = None
        self.lost = 0

    def update(self, arrival):
        """Return (time, index) for the next frame, which arrived at arrival."""
        if self._a and arrival - self._a[-1] >= SEGMENT_GAP:
            # A pause: a new line, the numbering carried on by its length.
            last, index, lost = self._a[-1], self.index, self.lost
            self.reset()
            self.index = index + max(1, round((arrival - last) / self.period)) - 1
            self.lost = lost
        self.index += 1
        self._k.append(self.index)
        self._a.append(arrival)
        if len(self._a) < MIN_FIT:
            return arrival, self.index
        self.phase, self.period = _fit(self._k, self._a, self.period)

        # A loss before the frame LOSS_WINDOW back (as retime: the least delay after it against
        # the least before it, on the nominal period, which the loss can't tilt as it does the
        # fit). It and the frames since are renumbered; the line fitted again.
        n = len(self._a)
        if n >= 2 * LOSS_WINDOW:
            c = n - LOSS_WINDOW
            a = np.array(self._a)
            if (a[c] - a[c - 1]) / self.nominal >= BURST:
                k = np.array(self._k)
                r = (a - a[0]) / self.nominal - (k - k[0])
                before = r[c - LOSS_WINDOW:c].min()
                step = int(np.floor(r[c:].min() - before + 1 - LOSS_THRESHOLD))
                if step >= 1:
                    gap = np.diff(a, prepend=-np.inf) / self.nominal
                    for j in range(_place(r - before, gap, c, step), n):
                        self._k[j] += step
                    self.index += step
                    self.lost += step
                    self.phase, self.period = _fit(self._k, self._a, self.period)
        return self.phase + self.period * self.index, self.index
