# 3D Cellular Automata

This is a project that I have created to learn about OpenGL and all that goes with by writing things from scratch.

### Warning
**This project is NOT optimized at all**, it maxes out my GPU (GTX1050 Mobile) and still runs only at maybe 5-10 fps without the cellular generation and some code disabled and maybe **0.5-1 fps when doing the generation** (Disabling generation and the indices update produces a very fluid render)
I've tried a lot of things to try and speed it up, but maybe I still have a wrong understanding of how meshes and vertices/indices should work.
As I said the main goal for this project wasn't to have things run smoothly but to learn about OpenGL

### Thanks
It would have been possible without @VictorGordan amazing tutorial
https://www.youtube.com/watch?v=45MIykWJ-C4
https://github.com/VictorGordan/opengl-tutorials

The "3D engine" I wrote is highly based on this tutorial with some changes like no lighting, no texture and auto-rotating the cubes.

### Demo video
Rule used 4/4/5/M:

[![Demo](https://img.youtube.com/vi/Ccv0VoVGOJg/0.jpg)](https://www.youtube.com/watch?v=Ccv0VoVGOJg)

### Improvements
There are quite a few improvements that could be implemented:
   - Don't hard-code the first part and last part of the rule the `4/4` and the `M` is hard-coded. Adding support for Von Neumann neighborhood calculation should be too difficult. The first part of the rule is hard-coded due to the computing time required to check if a vector contains a value (which multiplied by 70^3 is quite a lot)
   - Don't render individual cubes and remove duplicate vertices: The idea is to enlarge the cubes so that they touch and remove unnecessary vertices/indices they would greatly improve render time and less the amount of data to send to the GPU by quite a lot.
   - Maybe add a video export (Don't know if it's possible)