## renderTelemetryEmpty

Draws the PhaseSpace tab's Telemetry card with no platforms and no LEDs, before the first packet arrives.

## setConn

Records whether this page reaches its server (`'ok'`, `'bad'` or `'warn'` while connecting) and redraws the connection badge.

## pageAvailable

Returns whether a tab exists on this console (the server's `features`), so an address naming a tab the other console has falls back to this one's first tab.
