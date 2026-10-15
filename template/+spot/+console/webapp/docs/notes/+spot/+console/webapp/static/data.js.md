## createDataStore.store.saved

The runs in Saved Data, as `spot.data.listSaved` lists them (refreshed when the Results tab asks).

## createDataStore.store.loaded

The datasets loaded in MATLAB for the Results tab and the replays, in the order they were loaded.

## createDataStore.store.simulation

Whether there is a last simulation to load (MATLAB has its results), for the Results tab's **Last Simulation** button.

## createDataStore.store.byId

The loaded dataset with this id, or undefined.

## createDataStore.store.overlay

How a dataset is laid over the simulation's replay (Compare): `{on, offset}`, where offset is its time shift [s]; off and unshifted until set.

## createDataStore.store.setOverlay

Changes a dataset's overlay (`{on}` or `{offset}`) and tells every view that follows the store.

## createDataStore.store.on

Calls `fn` whenever the store changes (datasets loaded or unloaded, an overlay changed), and returns a function that stops it.
