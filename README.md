Fantasi
=======

Fantasi is a realtime raytracer built using OpenGL Compute shaders.  The intention is 
to maintain a certain level of physically-based rendering (to create gorgeous, 
photorealistic renders), while ensuring that it runs in realtime.

For some more details, check my website: thecodeboss.com

Windows only, I'm afraid.  I'll make it multi-platform when the inspiration strikes.

Feature Support
===============

It supports basic materials defined by their emissive, ambient, diffuse, and specular
colors, as well as additional parameters of shininess, refractive index, absorption
coefficient and dielectrics.  This material model will certainly be expanded and ironed
out a bit better in the future, but for now it is a great photorealistic framework.

Lighting uses properties of the Fresnel effect, and supports expected raytracing
reflections and shadows with ease.

Primitives currently supported are triangles, spheres, and metaballs.  If you're
unfamiliar with metaballs, they're essentially spheres that glob together.

Oh, it also has anti-aliasing.

Controls
========

'W', 'A', 'S', 'D': Forward, left, back, right (or you can use arrow keys).

'R': Increase number of reflections

'1', '2', '3': Set the anti-alias parameter

'Shift': Maps 'W' and 'S' (and corresponding arrow keys) to up/down instead of forward/back.

Future Development
==================

Sadly I have not used any acceleration structures.  The main obstacle is getting these
to run efficiently on the GPU; generally these structures are highly optimized with the
assumption that memory access is fast, which is not true on the GPU.  For the future,
I'll continue to look through papers and try to find a way to implement this.

The metaballs are slow due to ray-marching.  I've done some optimization already, however
I imagine there is quite a bit more optimization that can be done.  I may post more ideas
on this on my blog at some point, if I can organize the thoughts floating in my mind.

I was struck with the sudden motivation to write an XML parser, so I could store my scene
geometry somewhere other than Main.cpp.  So I wrote it - but then got lazy to actually
use the darn thing.  I'll do it at some point.

License and Restrictions
========================

Fantasi is free to use, modify, and distribute assuming this readme remains, with this
license unmodified.  Using Fantasi code for profit is prohibited unless given permission
to do so.  Generally speaking if you contact me about using some of the code for profit,
it will be acceptable, but doing so without contacting me is NOT.

Fantasi is provided AS IS, and the user assumes all risks involved with using the software.
I am not responsible for damaged software/hardware in any form, and I am not obligated to
provide bug fixes or new features (though if you contact me I will probably be helpful).
