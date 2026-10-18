"""Install the MATLAB Engine for Python for this user without pip.

    python3 install_engine.py <matlabroot>

pip's install of <matlabroot>/extern/engines/python only copies the
package it ships (dist/matlab: pure Python plus one shared library per
platform) and writes engine/_arch.txt saying where MATLAB is. This does
the same into the user's site-packages, for a Python without pip (a
distribution's python3, say). spot.console.ensureEngine runs it when
pip is missing.

Python 3.9 or newer.
"""

import os
import platform
import shutil
import site
import sys


def main():
    """Copy the package and write its _arch.txt; print where it went."""
    if len(sys.argv) != 2:
        sys.exit("usage: install_engine.py <matlabroot>")
    root = sys.argv[1]
    source = os.path.join(root, "extern", "engines", "python", "dist", "matlab")
    if not os.path.isdir(os.path.join(source, "engine")):
        sys.exit("%s has no MATLAB Engine for Python (extern/engines/python/dist)" % root)
    system = platform.system()
    if system == "Linux":
        arch = "glnxa64"
    elif system == "Windows":
        arch = "win64"
    elif system == "Darwin":
        arch = "maca64" if platform.machine() == "arm64" else "maci64"
    else:
        sys.exit("unsupported platform %s" % system)
    if not os.path.isdir(os.path.join(root, "bin", arch)):
        sys.exit("%s has no %s binaries" % (root, arch))

    # The package, replaced if there, and where MATLAB is (as setup.py writes it).
    packages = site.getusersitepackages()
    target = os.path.join(packages, "matlab")
    os.makedirs(packages, exist_ok=True)
    if os.path.isdir(target):
        shutil.rmtree(target)
    shutil.copytree(source, target)
    with open(os.path.join(target, "engine", "_arch.txt"), "w") as f:
        f.write("\n".join([arch, os.path.join(root, "bin", arch),
                           os.path.join(source, "engine", arch),
                           os.path.join(root, "extern", "bin", arch)]) + "\n")
    print("Installed the MATLAB Engine for Python in %s" % target)


if __name__ == "__main__":
    main()
