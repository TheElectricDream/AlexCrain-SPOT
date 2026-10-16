StreamData started as PhaseSpace's `example_rigid1.cc` sample and grew into the lab's PhaseSpace server: it runs on the lab console, reads the cameras through PhaseSpace's OWL library, and sends every frame to the platforms and the console. The [PhaseSpace tab](page:tab-phasespace) compiles and runs it; [Network, Addresses and Ports](page:lab-network) lists where its packets go.

## networkCleanup

Shuts Windows' socket library down (`WSACleanup`); nothing to do on Linux.

## setNonBlocking

Makes a socket non-blocking, so a send to a computer that isn't answering returns at once instead of holding up the frames to everyone else.

## openDestination

Opens the UDP socket for one destination: a 1 MB send buffer, sending from StreamData's own port (31535) so receivers that filter on the source port still accept it, connected to `address:port`, non-blocking. Each destination has a socket of its own, so a computer that is switched off can't fill a shared buffer and stop the packets to the others.

## main

The program. It reads its arguments (the frequency, an optional monitor port for the console's 3D view, `--udp`, `--dest` addresses), connects to the PhaseSpace server, defines the three rigid bodies from the marker file (`tracker_positions_phasespace.txt`), and then, for every frame: computes each platform's position, attitude and velocities (from PhaseSpace's own timestamps), collects every LED's position, writes the experiment clock into its slot once an experiment has started, and sends the packet to each destination and the monitor port. Lines on its standard input add or remove destinations (`dest add`, `dest remove`) and start or clear the experiment clock (`exp start <s>`, `exp clear`); the lab console sends them. It prints a status line regularly, and stops cleanly on Ctrl+C or SIGTERM. The comments in its body explain each part.
