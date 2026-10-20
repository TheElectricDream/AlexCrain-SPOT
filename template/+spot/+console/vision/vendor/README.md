# Vendored camera libraries

Libraries SPOT Vision runs that aren't on PyPI, copied here so `deploy_orin.sh` carries them to the
Orin (which needs no internet). Copied unchanged: change them upstream, then copy them again. The
audit leaves this folder out, as it does the page's `static/vendor`.

## ici9320: the ICI 9320P thermal camera

The `ici9320/` package (the SDK) and its FX2 firmware image (`ici9320/firmware/ici9320.ihx`, built
from the repository's `firmware/explorer` with fx2lib, LGPL), from

    https://github.com/TheElectricDream/ICI9320P
    commit a489e9edb3bf0a7d82a5a65392b106a7125f7fde (2026-10-16)

It needs `libusb1` (python-libusb1), numpy and, although the camera class doesn't use it, `pyusb`:
the package's `__init__.py` imports `fx2loader.py`, a PyUSB firmware loader. The SDK's tools,
firmware sources and captures stay in its repository. To update:

    git clone https://github.com/TheElectricDream/ICI9320P
    cp ICI9320P/ici9320/*.py vision/vendor/ici9320/
    cp ICI9320P/ici9320/firmware/ici9320.ihx vision/vendor/ici9320/firmware/

and put the new commit above. On the Orin, `python3 -m ici9320 info` (from `~/spot_vision/vendor`)
checks the camera by hand: firmware version and housing temperature.
