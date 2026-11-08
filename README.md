# 3D Solar System Simulation

A simple 3D solar system simulation built using C++ and OpenGL.

## Screenshots

### Solar System Overview
![Solar System Overview](screenshots/top.png)

### Planetary Orbits
![Planetary Orbits](screenshots/SideOrbit.png)

## Overview

This project visualizes the Sun and planets in a 3D environment. Their motion is calculated from their mutual gravitational attraction, so every body—including the Sun—can move in response to the others.

The simulation is an educational approximation rather than a precision ephemeris. Orbital paths emerge from the bodies' initial positions, masses, and velocities; they are not prescribed with sine and cosine functions.

## Features

- Procedurally generated 3D spheres.
- Multiple planets with individual sizes and colours.
- Pairwise Newtonian gravity between simulated bodies.
- Adjustable initial positions, masses, and velocities.
- 3D camera controls.
- Real-time rendering using OpenGL.

## Technologies Used

- C++
- OpenGL 3.3 Core
- GLFW
- GLAD
- GLM

## How It Works

Positions are measured in astronomical units (AU), masses in solar masses, and velocities in AU per year. The gravitational constant is expressed in the matching units:

```cpp
G = 4 * pi^2 AU^3 / (solar mass * year^2)
```

At each simulation step, the program calculates the gravitational force between every pair of bodies and derives their accelerations from those forces. Velocities and positions are then advanced using a semi-implicit Euler step. Each rendered frame is divided into 10 substeps.

Simulation time advances at 30 simulated days per real second. Initial velocities are important: gravity alone pulls bodies together, while a suitable sideways velocity is needed for an orbit. Since gravity is mutual, a planet also causes the Sun to move; both orbit their shared center of mass.

Each planet shares the same sphere mesh, with individual model matrices controlling its position and scale.

## Project Scope

This is an educational graphics programming project intended to explore:

- 3D coordinate systems
- Spherical geometry
- Model transformations
- Newtonian gravitation and orbital motion
- Numerical integration
- Procedural mesh generation
- Real-time rendering

The simulation is not intended for precision scientific use. It uses a simple numerical integrator and a visual representation whose sphere sizes are not to scale.

## Status

Currently in development.
