# Send Data to Orin, Check Connection

Two small subsystems that run late in the step (priorities 70 and 71) and only send.

## Send Data to Orin

![Send Data to Orin](images/sub_Send_Data_to_Orin.png)

**Reads:** `isSim`, `Univ_Time`, `RED/BLACK/BLUE_Measured_States`.

- **Data Store Read1** (`isSim`) → **If** `u1 == 0`: experiments only.
- **Subsystem**: **Mux** `[Univ_Time; RED states (9); BLACK states (9); BLUE states (9)]` = 28 doubles → **UDP Send**, the MATLAB Function `spot.hardware.udpSend('192.168.1.115', 46875, data)` (`spot.hardware.udpSend`): to the vision computer (the Orin at `.115`), every step. The Orin uses the true poses to evaluate (or train) its own estimate, which comes back through [Receive Data from Orin](page:diagram-orin-receive).

## Check Connection

![Check Connection](images/sub_Check_Connection.png)

**Reads:** `WhoAmI`, `isSim`.

- Three **If** blocks `WhoAmI == n & isSim == 0` pick this platform's **Change X Behavior**.
- Each one: **Constant1** = 1 → **UDP Send**, `spot.hardware.udpSend('255.255.255.255', 33001/33002/33003, int32(1))`: a one-word **heartbeat** broadcast to the lab network every step, on a port per platform (RED 33001, BLACK 33002, BLUE 33003). Anyone listening can tell that the platform's program is running (the old ground station did; the current lab console judges a program by its experiment clock on ports 48720–48722 and by `exp-status` instead, see [experiment.py](file:experiment.py)).

Both subsystems send blindly (UDP has no acknowledgement) and cost nothing when nobody listens.
