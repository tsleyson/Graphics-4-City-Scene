Graphics-4-City-Scene
=====================

Fourth assignment for ECS 175, to create a city scene.

I made this repository mostly to try out making a repository.

Alright, got the geometry done and ported to a form C can understand :p

Only took 12 hours in total, too. Gawsh, I'm sleepy z_z

https://www.dropbox.com/s/oud1815obghij9d/hw4.zip has all the files, including the original vmf. If you need to see anything in the vmf in detail, as it's plaintext, you can use this as a guide: https://developer.valvesoftware.com/wiki/VMF_documentation#Side Although I believe I have everything transfered over. There's a chance that I mighta deleted a face in the process. If I did, I can fix it tomorrow, but fair warning! I will be a bit entirely asleep until into the afternoon since I've been pulling crazy hours for the past few days. So if possible, you can try to use the files in the zip to fix any missing parts.

A breakdown of the files in the .zip:

project3.vmf: the original source sdk file
points.txt: removed all faces that will not be rendered in the final product (sourcesdk introduces faces that should never be rendered). These are marked with NODRAW textures, so there should be no NODRAW texture sides/faces in points.txt.
points_extract: removed the u/v texture coordinate system data, as well as other miscellaneous data such as face ID
points_array_1: textures tagged as something non-gibberish
points_array_2: I have no idea what I changed here
array_points: removed all texture data to isolate vert data
array_points_FINAL: fixed up the brackets to make it in a C++-acceptable format.
array_textures: isolated all the textures from points_array_2, and turned them into a C++-acceptable format
array_textures_FINAL: array_textures, but with a different file name to match the array_points_FINAL to avoid confusion
car: isolated data etc etc
car_points: contains the vert data for the car, which is what you want

Regarding the array points file, each building is in absolute coordinates relative to the ground. That is, we do not need to move anything here. The most we need to do is normalize the points, which will most likely be needed. You can just map it to [0-1] The max and min bounds of the map should be: max = [1536 1536 646.4], min = [-1536 -1536 0].

Once the points have been normalized, if you think it's needed, generating the structure should be a simple case of looping through the points, and generating the normals as we move along.

For the textures, there are 12 textures in total. We do not need every face to be assigned a texture, but I have it set up such that assigning a texture to every face is trivial at worst. Just use the array!

For the car, car_points contain the vertex data. We should just do per-vertex coloring as texturing would be difficult with our current time limits. Maybe make the glass RGB 228, 243, 248?

Also note that the car tire *SIDES* require a triangle fan. You can construct the triangle fan from the tire points following each TEX_TIRE_FRONT_BACK or whatever I declared it as. Everything else is in quad format though- just the tire sides are weird.

For the tutorials I mentioned earlier, http://codecolony.de/opengl.htm#Camera2

And for the camera movement, I was thinking of moving the camera along a line whose slope is defined in part by the glulookat() rotation potions. We can use the vector definition of a line, then move along the line when the user presses forward. http://tutorial.math.lamar.edu/Classes/CalcIII/EqnsOfLines.aspx

What else? I hope this will do it. Just for safety, I'm also attaching a copy of the zip + a 7z version incase dropbox doesn't work right.


