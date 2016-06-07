3D Interactive Simulation
=========================

This 3D Interactive Simulation illustrates the use of a swarm intelligence algorithm, inspired by
animal collective intelligence, to steer squads of autonomous vehicles in Virtual Reality and
Gaming applications.

The developed algorithm allows a minimal CPU cost for the computation of 3 sorts of steering
behaviors (3 sorts of formations). This algorithm follows the concept of Force Based Emergent
Steering (FBES) [1].

The 3D Interactive Simulation is based on OGRE (Object-Oriented Graphics Rendering Engine), a
scene-oriented flexible 3D engine, and Nvidia PhysX, a scalable multi-platform game physics
engine. It uses a mathematical library based on quaternions to handle analytic geometry in 3D
space.

The source code is written in C++ and HLSL. A full build for Windows x86 is included in the
repository.

A Visual C++ 2010 project file is also included, which uses the following environment variables:
- OGRE_HOME: OGRE SDK 1.7.3 root directory
- PHYSX_DIR: Nvidia PhysX SDK 2.8.4 root directory
- NXOGRE_DIR: NxOgre 1.6.3330 root directory
- CRITTER_DIR: NxOgre Critter root directory

[1] The term FBES was coined by S.L. Tomlinson in "The Long And Short Of Steering In
Computer Games", published in 2004 in the "International Journal of Simulation: Systems, Science
& Technology" Vol. 5 No. 1-2 (ISSN 1473-8031).
