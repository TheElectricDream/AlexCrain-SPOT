The Identify Platform block (`spot.hardware.IdentifyPlatform`): each platform's program finds out which platform it runs on, so the same build works on all three.

## getComputerIdentifier

The platform number for a computer name: `spot-red` 1 (Chaser), `spot-black` 2 (Target), `spot-blue` 3 (Obstacle), `spot-spare` 4, `spot-vision` 5, and 0 for any other name.

## WhoAmI

Reads this computer's name and returns its platform number (`getComputerIdentifier`). The diagram puts it in the `WhoAmI` data store, so each platform runs its own branch. If the name can't be read, it returns 1, the Chaser's number.
