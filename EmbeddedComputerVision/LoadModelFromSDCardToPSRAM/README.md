For situatiops where the model is too big to fit in the flash (and consequently too big for the RAM).

ESP-WROVER-E has 8 megabytes of external RAM, with 4 of those being easily accesible using psramInit(), ps_malloc() and similar functions.
Those can be used to save the model and its required tensors.

1) Read the model from SD card
2) Allocate the memory in PSRAM for the model 
3) Allocate the estimated memory for the tensor arena
4) Run the model in order to calculate the memory usage
5) Print the actual memory usage