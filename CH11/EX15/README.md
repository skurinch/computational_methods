The four rate equations that correspond to the SEIR disease model are
$$
\frac{dS}{dt} = bN -\beta S + \nu R - dS
$$
$$
\frac{dE}{dt} = \beta S - \lambda E - dE
$$
$$
\frac{dI}{dt} = \lambda E - \gamma I - dI
$$
$$
\frac{dR}{dt} = \gamma I - \nu R - dR
$$
If $b = d$, then the birth rate is equal to the death rate and
$$
\frac{dN}{dt} = \frac{dS}{dt} + \frac{dE}{dt} + \frac{dI}{dt} + \frac{dR}{dt} = (b - d) N = 0
$$
so the population is static.

The model assumes that being infected does not increase an individual's probability of dying, in which the case population would decline due to disease-induced mortality even when $b=d$.