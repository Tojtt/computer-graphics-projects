The project write-up is at [Project 1: Rasterizer]((https://tojtt.github.io/computer-graphics-projects/proj1/index.html))

<p>
  <img src="https://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/3_1.jpg" width="800px" align="middle"/>
</p>

In this assignment you will implement a simple rasterizer, including features like drawing triangles, supersampling, hierarchical transforms, and texture mapping with antialiasing. At the end, you'll have a functional vector graphics renderer that can take in a simplified version of SVG (Scalable Vector Graphics) files, which are widely used on the internet.

## Project Structure

The project has 6 tasks, worth a total of 100 possible points. Some require only a few lines of code, while others are more substantial.

* Task 1: Drawing Single-Color Triangles (20 pts)
* Task 2: Antialiasing by Supersampling (20 pts)
* Task 3: Transforms (10 pts)

* Task 4: Barycentric coordinates (10 pts)
* Task 5: "Pixel sampling" for texture mapping (15 pts)
* Task 6: "Level sampling" with mipmaps for texture mapping (25 pts)

* Part 7: Extra Credit - Draw Something Creative!


## Logistics

### Deadline
* **Project 1** is due **Tuesday 2/18, 11:59PM**. The [**checkpoint quiz**](https://forms.gle/DQKEiZzFvFxKYMU69) is due **Tuesday 2/11, 11:59PM.** For the project, both your code and write-up need to be turned in for your submission to be complete. Assignments which are turned in after 11:59pm will use one of your late days -- there are no late minutes or late hours.


### Getting started

First, accept the assignment in your CS184 website profile, following the instructions from GitHub Classroom. Then, clone the generated repo. Make sure you clone _your repo_, and not the class skeleton.

    $ git clone --recurse-submodule <YOUR_PRIVATE_REPO>
    
Also ensure GitHub Pages is enabled for your assignment.

Please consult this [how to build and submit assignments for CS 184](https://cs184.eecs.berkeley.edu/sp20/article/11/building-and-submitting-cs184-as) article for more information on how to setup and build the project.

As you go through the assignment, [refer to the write-up guidelines and deliverables section below](#rubric). **It is recommended that you complete each section's write-up as you finish that section.** It's generally not a good idea to wait until the end to start your writeup. You may also find it helpful to skim the rubric before beginning your work.

*Important: To avoid compression artifacts in your images, please do not convert the PNG screenshot images saved by the GUI into JPG or other formats!  PNG images are losslessly compressed.* 

Finally, you may find the following resources helpful: 

**[C++ Guide](https://cs184.eecs.berkeley.edu/sp20/article/9/c-intro)** for some quick tips and tricks on getting started with C++. A slightly more detailed C++ guide can be [found here](https://github.com/Bryce-Summers/Writings/blob/master/Programming%20Guides/C_plus_plus_guide.pdf).

**[Images as Data](https://cs184.eecs.berkeley.edu/sp20/article/7/images-as-data)** on how images and colors are often represented in code.

**[Vectors and Matrices in the CGL Library](https://cs184.eecs.berkeley.edu/sp20/article/12/primer-on-using-vectors-and-matr)** for a quick refresher on declaring and using vectors and matrices.

<p>
  <img src="https://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/3_7.jpg" width="800px" align="middle"/>
</p>

### Using the GUI

You can run the executable with the command

    ./draw [path to svg file/folder to render]

For example, you could run this command: 
    
    ./draw ../svg/basic/test1.svg

You'll see a flower composed of blue dots, based on point and line rasterization provided in the starter code.  Most other SVG files won't render correctly until you work through the assignment. Here are the keyboard shortcuts available (some depend on you implementing various parts of the assignment):

|Key | Action|
|:-----:|------|
|  <kbd>space</kbd>  | return to original viewpoint|
| <kbd>-</kbd>  | decrease sample rate|
| <kbd>=</kbd> | increase sample rate|
| <kbd>Z</kbd> | toggle the pixel inspector|
| <kbd>P</kbd> | switch between texture filtering methods on pixels|
| <kbd>L</kbd> | switch between texture filtering methods on mipmap levels|
| <kbd>S</kbd> | save a *PNG* image screenshot in the current directory|
| <kbd>1</kbd> - <kbd>9</kbd>  | switch between svg files in the loaded directory|

The argument passed to `draw` can either be a single file or a directory containing multiple *svg* files, as in

    ./draw ../svg/basic/

If you load a directory with up to 9 files, you can switch between them using the number keys 1-9 on your keyboard.



### Familiarize Yourself with the Starter Code

Most of your modifications will be constrained to implementing or modifying functions in `rasterizer.cpp`, `transforms.cpp` and `texture.cpp`. 

In addition to modifying these, you will need to understand other source and header files as you work through the project.  As one example, the starter code for this and future assignments use the CGL library.  For this assignment, you may want to familiarize yourself with classes defined in `vector2D.h`, `matrix3x3.h` and `color.h`.

Here is a brief sketch of what happens when you launch `draw`: An `SVGParser` (in `svgparser.h/cpp`) reads in the input *svg* file(s), launches a OpenGL `Viewer` containing a `DrawRend` (in `drawrend.h/cpp`) renderer, which enters an infinite loop and waits for input from the mouse and keyboard. In `DrawRend::redraw()` function, the high-level drawing work is done by the various `SVGElement` child classes (in `svg.h/cpp`), which then pass their low-level point, line, and triangle rasterization data to appropriate methods of a `Rasterizer` class.

In fact, now might be a good time to finish the [Checkpoint Quiz](https://forms.gle/m77tfLskx55wBCb49) due Tuesday 2/11.

### A Simple Example: Drawing Points


You are given starter code that already implements drawing of 2D points. To see how this works, begin by taking a look at `SVG::draw()` in `svg.h`. The SVG object draws all elements in the SVG file via a sequence of calls to their `draw()` functions.  Each element type calls an appropriate draw function on a `Rasterizer` object. In the case of the `Point` element type, `Point::draw()` eventually calls the concrete draw function implemented in `RasterizerImp::draw_point()` in `rasterizer.cpp`.  The position of SVG elements in an SVG file is defined in a local coordinate frame, so `Point::draw()` transforms the point's position into screen-space coordinates before passing it to `RasterizerImp::draw_point()`. 

The function `Rasterizer::draw_point()` is responsible for actually drawing the point. In this assignment we define screen space for an output image of size `(target_w, target_h)` as follows:

- `(0, 0)` corresponds to the top-left of the output image
- `(target_w, target_h)` corresponds to the bottom-right of the output image
- **Please assume that screen sample positions are located at half-integer coordinates in screen space. That is, the top-left sample point is at coordinate (0.5, 0.5), and the bottom-right sample point is at coordinate (target_w-0.5, target_h-0.5).**

![Sample locations](https://github.com/stanford-cs248/draw-svg/raw/master/misc/coord_1spp.png?raw=true)

You may also wish to read [this resource](https://cs184.eecs.berkeley.edu/sp19/article/7/images-as-data) for more detail on how colors and images are represented as data. 

To rasterize points, we adopt the following rule: a point covers at most one screen sample: the closest sample to the point in screen space. This is implemented as follows, assuming (x, y) is the screen-space location of a point.

```
int sx = (int) floor(x);
int sy = (int) floor(y);
```

Of course, the code should not attempt to modify the render target buffer at invalid pixel locations.

```
if ( sx < 0 || sx > target_w ) return;
if ( sy < 0 || sy > target_h ) return;
```

If the points happen to be on screen, we fill in the pixel with the RGB color associated with the point.

```
  rgb_framebuffer_target[3 * (y * width + x)    ] = (unsigned char)(c.r * 255);
  rgb_framebuffer_target[3 * (y * width + x) + 1] = (unsigned char)(c.g * 255);
  rgb_framebuffer_target[3 * (y * width + x) + 2] = (unsigned char)(c.b * 255);
```

(Note: In this assignment, we do not support partial transparency or alpha blending, even though this is part of the SVG file format.)


## Project Tasks

### Task 1: Drawing Single-Color Triangles (20 pts)
[**Relevant lecture: 2**](https://cs184.eecs.berkeley.edu/sp19/lecture/2/rasterization)

In this task, you will implement the `rasterize_triangle` function in `rasterizer.cpp`. Your solution should:

* Rasterize the triangle by using the sampling methods described in class. 
* For each pixel, please perform the point-in-triangle tests with a sample point in the center of the pixel, not the corner. The coordinates of your sample should be equal to an integer point plus (.5,.5).
* In Part 2 you will implement sub-pixel supersampling, but here you should just sample once per pixel and call the `fill_pixel()` helper function. Follow the example in the `rasterize_point` function in the starter code. 
* To receive full credit, your implementation should assume that a sample on the boundary of the triangle is to be drawn. You are encouraged but not required to implement the OpenGL edge rules for samples lying exactly on an edge. Do make sure that none of your edges are left un-rasterized.
* Your implementation should be at least as efficient as sampling only within the bounding box of the triangle (not simply every pixel in the framebuffer).
* Your code should draw the triangle regardless of the winding order of the vertices (i.e. clockwise or counter-clockwise). Check `svg/basic/test6.svg`.

When finished, you should be able to render test SVG files with single-color polygons (which are triangulated into triangles elsewhere in the code before being passed to your function.  Files `basic/test3.svg`, `basic/test4.svg`, `basic/test5.svg`, and `basic/test6.svg` should render correctly.

For convenience, here is a list of functions you will need to modify:

1. `RasterizerImp::rasterize_triangle()` function in `rasterizer.cpp`.

**Extra Credit:** Make your triangle rasterizer super fast (e.g., by factoring redundant arithmetic operations out of loops, minimizing memory access, and not checking every sample in the bounding box). Write about the optimizations you used. Use `clock()` to get timing comparisons between your basic and optimized implementations.



### Task 2: Antialiasing by Supersampling (20 pts)
[**Relevant lecture: 3**](https://cs184.eecs.berkeley.edu/sp19/lecture/3/antialiasing)

Use supersampling to antialias your triangles. The `sample_rate` parameter in `DrawRend` (adjusted using the `-` and `=` keys) tells you how many samples to use per pixel.

The image below shows how sampling four times per pixel produces a better result than just sampling once.  The fraction of the supersamples within the triangle yields a smoother edge.

<p>
  <img src="https://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/3_1.jpg" width="500px" align="middle"/>
</p>

To implement supersampling, please sample at `sqrt(sample_rate) * sqrt(sample_rate)` grid locations distributed over the pixel area.  

Suggestions for this task:

* You will need to manage appropriate memory to store your supersample data.  We recommend that you use the `RasterizerImp::supersample_buffer` vector (see file `rasterizer.h`) for this purpose.  It depends on your algorithm, but it is likely that the size of the supersample buffer you need will depend on the framebuffer dimensions (which changes when the window is resized) and the supersampling rate (which changes with keystrokes as described above).  You will need to update the size of the buffer dynamically. There are hints below and in the code for where you may want to manage the size of your buffer.
* Clear the values in your supersample buffer memory and/or framebuffer appropriately at the beginning of redrawing the frame. This is erasing the frame before you start drawing. 
* Update your `rasterize_triangle` function to perform supersampling into your supersample buffer memory. There are multiple ways to organize the data in your supersample buffer, and the choice is up to you. One reasonable way to think about supersampling is simply rasterizing an image that is higher resolution, then downsampling the higher resolution image to the output resolution of the framebuffer.  In this way of thinking, your supersample buffer is just a temporary, higher-resolution framebuffer.  For example, 4x4 supersampling with a 1000x1000 pixel framebuffer means rasterizing a 4000x4000 (high-res) image of the scene into your supersample buffer.  After you rasterize the high-res image, you need to downsample to 1000x1000 final pixels by averaging down the 4x4 grid of sample values that are related to each output pixel.  In this way of thinking, you need to store more memory in order to perform the high-res supersampled rasterization.  (Test your understanding: can you achieve the same results without needing more memory, and if so, what are the engineering tradeoffs?)
* At the end of rasterizing all the scene elements, you will need to populate the framebuffer from your supersamples.  This is sometimes called resolving the samples into the framebuffer.  Notice that the the `RasterizerImp::resolve_into_framebuffer` function is called as the last step in rendering the frame, so you may wish to implement this part of your algorithm here.   
* Note that you will need to convert between different color datatypes. `RasterizerImp::rgb_framebuffer_target` stores a pointer to the framebuffer pixel data that is finally drawn to the display. `rgb_framebuffer_target` is an array of 8-bit values for each of the R, G and B components of each pixel's color -- this is the compact data format expected by most real graphics systems for drawing to the display.  In contrast, the `RasterizerImp::supersample_buffer` variable that we suggest you use for your supersample memory is an array of `Color` objects that store R, G and B internally as floating point values.  You may wish to familiarize yourself with the `Color` class.  You may need to convert between these datatypes.  Watch out for floating point to integer conversion errors, such as rounding and overflow. 
* You will likely find that points and lines stop rendering correctly after your supersampling modifications. The starter code point and line rasterization use `RasterizerImp::supersample_buffer` -- update this function if needed to restore functionality.  You do NOT need to antialias points and lines.

Once your implementation is complete, your triangle edges should look noticeably smoother when using more than one sample per pixel! You can examine the differences closely using the pixel inspector (see controls listed above). Also note that, it may take several seconds to switch to a higher sampling rate.

For convenience, here is a list of functions you will likely want to use or modify.

1. For managing supersample buffer memory: `RasterizerImp::supersample_buffer`,  `RasterizerImp::set_sample_rate()`, `RasterizerImp::set_framebuffer_target()`, `RasterizerImp::clear_buffers()` in `rasterizer.h/cpp`.
2. To implement triangle supersampling: `RasterizerImp::rasterize_triangle()` in `rasterizer.cpp`.
2. For resolving supersamples to framebuffer: `RasterizerImp::resolve_to_framebuffer()`.

**Extra Credit:** Implement an alternative sampling pattern, such as jittered or low-discrepancy sampling. Create comparison images showing the differences between grid supersampling and your new pattern. Try making a scene that contains aliasing artifacts when rendered using grid supersampling but not when using your pattern. 


### Task 3: Transforms (10 pts)
[**Relevant lecture: 4**](https://cs184.eecs.berkeley.edu/sp19/lecture/4/transforms)
<!--[**Relevant lecture: 4**](https://cs184.org/lecture/transforms-1)-->

Implement the three transforms in the *transforms.cpp* file according to the [SVG spec](https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/transform). The matrices are 3x3 because they operate in homogeneous coordinates -- you can see how they will be used on instances of `Vector2D` by looking at the way the `*` operator is overloaded in the same file.

Once you've implemented these transforms, *svg/transforms/robot.svg* should render correctly, as follows:

<img src="https://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/3_.jpg" width="400px" align="middle"/>

For convenience, here is a list of functions in *transforms.cpp* you will need to modify:

1. `translate`
2. `scale`
3. `rotate`

**Extra Credit:** Add an extra feature to the GUI. For example, you could make two unused keys to rotate the viewport. Save an example image to demonstrate your feature, and write about how you modified the SVG to NDC and NDC to screen-space matrix stack to implement it.


### Task 4: Barycentric coordinates (10 pts)
[**Relevant lecture: 5**](https://cs184.eecs.berkeley.edu/sp19/lecture/5/texture)
<!--[**Relevant lecture: 6**](https://cs184.eecs.berkeley.edu/lecture/texture)-->

Implement `RasterizerImp::rasterize_interpolated_color_triangle(...)` to draw a triangle with colors defined at the vertices and interpolated across the triangle area using barycentric interpolation.  

Once Part 4 is done, you should be able to see a color wheel in *svg/basic/test7.svg*.

For convenience, here is a list of functions you will need to modify:

1. `RasterizerImp::rasterize_interpolated_color_triangle(...)`

<p>
  <img src="https://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/3_4.jpg" width="800px" align="middle"/>
</p>

### Task 5: "Pixel sampling" for texture mapping (15 pts)
[**Relevant lecture: 5**](https://cs184.eecs.berkeley.edu/sp19/lecture/5/texture)

Implement `RasterizerImp::rasterize_textured_triangle(...)` to draw a triangle with colors defined by texture mapping with the given 2D texture coordinates at each vertex and the given `Texture` image.  Here in Task 5 you will implement texture sampling on the full-resolution texture image using nearest neighbor and bilinear interpolation, as described in lecture. 

The GUI toggles `RasterizerImp`'s `PixelSampleMethod` variable `psm` using the `'P'` key. When `psm == P_NEAREST`, you should use nearest-pixel sampling, and when `psm == P_LINEAR`, you should use bilinear sampling.  Please do so by implementing `Texture::sample_nearest` and `Texture::sample_bilinear` functions and calling them from `RasterizerImp::rasterize_textured_triangle(...)`.  This approach will allow you to reuse these functions for trilinear texture filtering  in Task 6. 

Once Part 5 is done, you should be able to rasterize the *svg* files in *svg/texmap/*, which rely on texture maps.

Notes:

* The `Texture` struct in `texture.h` stores a mipmap, as described in lecture, of texture images in decreasing resolution, in the `mipmap` variable. Each texture image is stored as an object of type `MipMap`.
* `MipMap::texels` stores the texture image pixels in the typical RGB format described above for framebuffer pixels.
* `MipMap::get_texel(...)` may be helpful.

For convenience, here is a list of functions you will need to modify:

1. `RasterizerImp::rasterize_textured_triangle`
2. `Texture::sample_nearest`
3. `Texture::sample_bilinear`


### Task 6: "Level sampling" with mipmaps for texture mapping (25 pts)
[**Relevant lecture: 5**](https://cs184.eecs.berkeley.edu/sp19/lecture/5/texture)

Finally, update `RasterizerImp::rasterize_textured_triangle(...)` to support sampling different `MipMap` levels. The GUI toggles `RasterizerImp`'s `LevelSampleMethod` variable `lsm` using the `'L'` key.

* When `lsm == L_ZERO`, you should sample from the zero-th `MipMap`, as in Part 5.
* When `lsm == L_NEAREST`, you should compute the nearest appropriate `MipMap` level using the one-pixel difference vectors `du` and `dv` and pass that level as a parameter to the nearest or bilinear sample function.
* When `lsm == L_LINEAR`, you should compute the `MipMap` level as a continuous number. Then compute a weighted sum of using one sample from each of the adjacent `MipMap` levels as described in lecture.  Please implement this in 

Implement `Texture::get_level` as a helper function. You will need $(\frac{du}{dx}, \frac{dv}{dx})$ and $(\frac{du}{dy},\frac{dv}{dy})$ to calculate the correct `MipMap` level. In order to get these values corresponding to a point $p = (x,y)$ inside a triangle, you must:

1. calculate the barycentric coordinates of $(x+1,y)$ and $(x,y+1)$ in `rasterize_textured_triangle`, passing these to `tri->color` as the variables `p_dx_bary` and `p_dy_bary`,
2. calculate the *uv* coordinates `sp.p_dx_uv` and `sp.p_dy_uv` inside `tri_color`,
3. calculate the difference vectors `sp.p_dx_uv - sp.p_uv` and `sp.p_dy_uv - sp.p_uv` inside `Texture::get_level`, and finally
4. scale up those difference vectors respectively by the width and height of the full-resolution texture image.

With these, you can proceed with the calculation from the lecture slides.

Notes:

* The `lsm` and `psm` variables can be set independently and interact independently. In other words, all combinations of `psm==[P_NEAREST, P_LINEAR] x lsm==[L_ZERO, L_NEAREST, L_LINEAR]` are valid.
* When `lsm == L_LINEAR` and `psm == P_LINEAR`, this is known as trilinear sampling, or trilinear texture filtering, as described in lecture. 

For convenience, here is a list of functions you will need to modify:

1. `RasterizerImp::rasterize_textured_triangle`
2. `Texture::sample`
3. `Texture::get_level`

**Extra Credit:** Implement [anisotropic filtering](https://en.wikipedia.org/wiki/Anisotropic_filtering) or [summed area tables](https://en.wikipedia.org/wiki/Summed_area_table). Show comparisons of your method to nearest, bilinear, and trilinear sampling. Use `clock()` to measure the relative performance of the methods.


## Part 7: Extra Credit - Draw Something Creative!

Use your newfound powers to render something fun. You can look up the svg specifications online for matrix transforms and for Point, Line, Polyline, Rect, Polygon, and Group classes. The ColorTri and TexTri are our own inventions, so you can intuit their parameters by looking at the svgparser.cpp file. You can either try to draw something "by hand" or try to output an interesting pattern programmatically. For example, we wrote some simple programs to generate the texture mapped *svg* files in the *texmap* directory as well as the color wheel in *basic/test7.svg*.

Flex your right or left brain -- either show us your artistic side, or generate awesome procedural patterns with code. This could involve a lot of programming either inside or outside of the codebase! If you write a script to generate procedural *svg* files, include it in your submission and briefly explain how it works.

**Tips and guidelines for your submission:**

* Your resulting *png* screenshot should be 800x800 resolution. Keep this in mind when writing your *svg* file.
* Use the GUI's `'S'` functionality to save your screenshot as a *png*. Don't take your own screenshot of your rasterized result, or you'll ruin the quality of your hard work!
* Note: The rasterizer cannot display svg Path elements, so do not include any curves in the svg you wish to load.

Students will vote on their favorite submissions and the top voted submission(s) will receive extra credit! More details regarding the art competition will be announced next week. Stay in tune!

## Submission

Please consult this article on [how to build and submit assignments for CS 184](https://cs184.eecs.berkeley.edu/sp20/article/11/building-and-submitting-cs184-as).

You will submit your code as well as some deliverables (see below) in a webpage project write-up.

### Project write-up guidelines and instructions

We have provided a simple HTML skeleton in *index.html* found within the *docs* directory to help you get started and structure your write-up.

You are also welcome to create your own webpage report from scratch using your own preferred frameworks or tools. However, **please follow the same overall structure as described in the deliverables section below**.

The goals of your write-up are for you to (a) think about and articulate what you've built and learned in your own words, (b) have a write-up of the project to take away from the class. Your write-up should include:

* An overview of the project, your approach to and implementation for each of the parts, and what problems you encountered and how you solved them. Strive for clarity and succinctness.
* On each part, make sure to include the results described in the corresponding Deliverables section in addition to your explanation. If you failed to generate any results correctly, provide a brief explanation of why.
* The final (optional) part for the art competition is where you have the opportunity to be creative and individual, so be sure to provide a good description of what you were going for and how you implemented it.
* Clearly indicate any extra credit items you completed, and provide a thorough explanation and illustration for each of them.

The write-up is one of our main methods of evaluating your work, so it is important to spend the time to do it correctly and thoroughly. Plan ahead to allocate time for the write-up well before the deadline.

### <a name="rubric"></a>Project Write-up Deliverables and Rubric

Note that this rubric is rather coarse. The content and quality of your write-up are extremely important, and you should make sure to at _least_ address all the points listed below. The extra credit portions are intended for students who want to challenge themselves and explore methods beyond the fundamentals, and are not worth a large amount of points. In other words, don't necessarily expect to use the extra credit points on these projects to make up for lost points elsewhere. 

#### Overview
Give a high-level overview of what you implemented in this project. Think about what you've built as a whole. Share your thoughts on what interesting things you've learned from completing the project.

#### Task 1 (20 pts)
* Walk through how you rasterize triangles in your own words. 
* Explain how your algorithm is no worse than one that checks each sample within the bounding box of the triangle.
* Show a *png* screenshot of *basic/test4.svg* with the default viewing parameters and with the pixel inspector centered on an interesting part of the scene.
* *Extra credit:* Explain any special optimizations you did beyond simple bounding box triangle rasterization, with a timing comparison table (we suggest using the c++ `clock()` function around the `svg.draw()` command in `DrawRend::redraw()` to compare millisecond timings with your various optimizations off and on).

#### Task 2 (20 pts) 
* Walk through your supersampling algorithm and data structures. Why is supersampling useful? What modifications did you make to the rasterization pipeline in the process? Explain how you used supersampling to antialias your triangles.
* Show *png* screenshots of *basic/test4.svg* with the default viewing parameters and sample rates 1, 4, and 16 to compare them side-by-side. Position the pixel inspector over an area that showcases the effect dramatically; for example, a very skinny triangle corner. Explain why these results are observed.
* *Extra credit:* If you implemented alternative antialiasing methods, describe them and include comparison pictures demonstrating the difference between your method and grid-based supersampling.

#### Task 3 (10 pts)

* Create an updated version of *svg/transforms/robot.svg* with cubeman doing something more interesting, like waving or running. Feel free to change his colors or proportions to suit your creativity. Save your *svg* file as *my_robot.svg* in your *docs/* directory and show a *png* screenshot of your rendered drawing in your write-up. Explain what you were trying to do with cubeman in words.

#### Task 4 (10 pts)
* Explain barycentric coordinates in your own words and use an image to aid you in your explanation. One idea is to use a *svg* file that plots a single triangle with one red, one green, and one blue vertex, which should produce a smoothly blended color triangle.
* Show a *png* screenshot of *svg/basic/test7.svg* with default viewing parameters and sample rate 1. If you make any additional images with color gradients, include them.

#### Task 5 (15 pts)
* Explain pixel sampling in your own words and describe how you implemented it to perform texture mapping. Briefly discuss the two different pixel sampling methods, nearest and bilinear.
* Check out the *svg* files in the *svg/texmap/* directory. Use the pixel inspector to find a good example of where bilinear sampling clearly defeats nearest sampling. Show and compare four *png* screenshots using nearest sampling at 1 sample per pixel, nearest sampling at 16 samples per pixel, bilinear sampling at 1 sample per pixel, and bilinear sampling at 16 samples per pixel.
* Comment on the relative differences. Discuss when there will be a large difference between the two methods and why.

#### Task 6 (25 pts)
* Explain level sampling in your own words and describe how you implemented it for texture mapping.
* You can now adjust choosing between pixel sampling and level sampling as well as adjust the number of samples per pixel. Analyze the tradeoffs between speed, memory usage, and antialiasing power between the various techniques at different zoom levels.
* Show at least one example (using a *png* file you find yourself) comparing all four combinations of one of `L_ZERO` and `L_NEAREST` with one of `P_NEAREST` and `P_LINEAR` at a zoomed out viewpoint.
  * To use your own *png*, make a copy of one of the existing *svg* files in *svg/texmap/* (or create your own modelled after one of the provided *svg* files). Then, near the top of the file, change the texture filename to point to your own *png*. From there, you can run ./draw and pass in that svg file to render it and then save a screenshot of your results.
* *Extra credit:* If you implemented any extra filtering methods, describe them and show comparisons between your results with the other above methods.

#### (Optional) Part 7
* Save your best *svg* file as *competition.svg* in your *docs/* directory, and show us a 800x800 *png* screenshot of it in your write-up!
* Explain how you did it. If you wrote a script to generate procedural *svg* files, include it in your submission in the *src/* directory and briefly explain how it works.

### Website tips and advice

* Your main report page should be called *index.html*.
* Be sure to include and turn in all of the other files (such as images) that are linked in your report!
* Use only *relative* paths to files, such as `"./images/image.jpg"`
* Do *NOT* use absolute paths, such as `"/Users/student/Desktop/image.jpg"`
* Pay close attention to your filename extensions. Remember that on UNIX systems (such as the instructional machines), capitalization matters. `.png != .jpeg != .jpg != .JPG`
* Be sure to adjust the permissions on your files so that they are world readable. For more information on this please see this [tutorial](http://www.grymoire.com/Unix/Permissions.html").
* Start assembling your webpage early to make sure you have a handle on how to edit the HTML code to insert images and format sections.


