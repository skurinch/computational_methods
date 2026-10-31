`PhotonGenerator` generates random photon wavelengths and impact positions, then calls `traceRay()` for each photon.
1. It calculates entry refraction using `refract()` which uses Snell's law,
2. an internal reflection using `reflect()` and `angleDegrees()`,
3. and refraction as the photon exits.

The `main()` function coordinates the simulation: 
1. Initializes MPI and identifies each process.
2. Divides the total photon count among the processes.
3. Builds an interpolator from the tabulated wavelength-dependent refractive indices of water, following `CH4/EX7`
4. Runs the photon calculation on each process.
5. Uses MPI_Reduce() to sum the local histograms on the master process.
6. Finalizes MPI and, on the master process, opens the Qt window to display the sampled photons with `RainbowWidget`.

`wavelengthColor()` converts a wavelength in the visible range into an [approximate RGB color](https://www.eureca.de/5116-1-Bruton-color-mapping.html).
