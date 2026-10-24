This code simulates flocking behavior using three simple rules: cohesion, separation, and alignment. Each bird responds to nearby neighbors 
rather than following a leader or coordinating with the entire flock. Cohesion steers it toward the average position of its neighbors, separation 
pushes it away from birds that get too close, and alignment adjusts its velocity toward the average velocity of nearby birds. These rules are 
applied continuously to update each bird’s movement, producing collective flocking behavior from local interactions. For separation, the code compares each bird with every other bird and checks whether their distance falls within the protected range. 
If another bird is too close, the code accumulates a repulsive vector pointing away from it. It adds the total 
repulsion to the bird’s velocity, steering it away from crowded areas.
