CS184 Assignment 2 Spring 2023
==============================

![](http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_2.jpg)

In this assignment, you will explore topics on geometric modeling covered in lecture. You will build Bezier curves and surfaces using de Casteljau algorithm, manipulate triangle meshes represented by half-edge data structure, and implement loop subdivision. For possible extra credit, you can optionally design your own polygon mesh using free modeling programs, such as [Blender](http://blender.org).

Deadlines
---------

Component

Deadline

Checkpoint Quiz

02/21/2023

Assignment 2

02/28/2023

Project 2 is due **Tuesday 02/28/2023, 11:59PM**. You have a total of 5 slip days to use on assignments throughout the entire semester, but you do not have slip days for checkpoint quizzes. Assignments submitted after 11:59PM are considered as a full day late. There is no late minutes or late hours.

Your final assignment submission must include **both** your final code and final write-up. Please consult this article on [how to submit assignment for CS184](https://cs184.eecs.berkeley.edu/sp23/docs/submitting-assignments).

Partners
--------

You can optionally work with a partner on this project. If you work with a partner, both of you should join the same team on Github Classroom. You should also produce 1 writeup and 1 gradescope submission with the partner added on.

Checkpoint Quiz
---------------

**There will be a checkpoint quiz for this project, due Tuesday 2/21, 11:59PM and is available on Gradescope.** This short quiz is intended to help you get started on project 2. The content necessary for answering all questions can be found on the project spec or in lecture material.

This checkpoint can be used to gain "buffer points" on the assignment. As long as you get at least 4 out of the 7 questions correct, AND submit a valid screenshot of your project building, you'll get the full 5 buffer points. No buffer points will be given if you did not meet these requirements. These 5 buffer points will be added to your project 2 grade, capped at increasing your score to 100. For example, if you earned 96 points on the base project and 2 points of extra credit, the 5 buffer points will bring your score up to 100, and the extra credit will bring your total score to 102.

### Project Parties

We will have 4 project parties:

Date/Time

Location

Friday 2/17 1-3pm

Berkeley Way West 1213

Wednesday 2/22 7-9pm

Cory 400

Friday 2/24 1-3pm

Berkeley Way West 1213

Monday 2/27 4:30-6:30pm

Cory 299

Tuesday 2/28 5-7pm

Cory 293

### Academic honesty

Please do not post code to a public GitHub repository, even after the class is finished, since these assignments will be reused both here and at other universities in the future.

Getting Started
---------------

As in Assignment 1, you should accept this assignment on your CS184 website profile, follow the instructions on GitHub Classroom, and clone the generated repo (**not** the class skeleton). Make sure you enable GitHub Pages for your assignment.

    $ git clone <YOUR_PRIVATE_REPO>
    

Please consult this article on [how to build assignments for CS184](https://cs184.eecs.berkeley.edu/sp23/docs/building-assignments).

**We recommend that you accumulate deliverables into your write-up as you work through each part this assignment.** We have included write-up instructions at the end of each part of this assignment, as well as [a compilation of all write-up instructions](#rubric) at the end.

Also, we have created a website template for Project 2 `docs/index.html` which contain all write-up questions and some template HTML codes (e.g. including figures). Please feel free to use it for your submission.

Assignment Structure
--------------------

This assignment has 7 parts, divided into 3 sections, and is worth a total of 90 possible points. Some parts require only a few lines of code, while others are more substantial.

**[Section I: Bezier Curves and Surfaces](#s1)**

*   [Part 1: Bezier curves with 1D de Casteljau subdivision (10 pts)](#p1)
*   [Part 2: Bezier surfaces with separable 1D de Casteljau (15 pts)](#p2)

**[Section II: Triangle Meshes and Half-Edge Data Structure](#s2)**

*   [Part 3: Area-weighted vertex normals (10 pts)](#p3)
*   [Part 4: Edge flip (15 pts)](#p4)
*   [Part 5: Edge split (15 pts)](#p5)
*   [Part 6: Loop subdivision for mesh upsampling (25 pts)](#p6)

**[Section III: Potential Extra Credit - Art Competition: Model something Creative](#s3)**

Running the Executable
----------------------

The general command for running the executable `meshedit` is the following:

    ./meshedit <PATH_TO_FILE>
    

Note that `meshedit` expects different file formats for different parts of this assignment, as detailed in the table below:

Part / Section

Expected File Format

[Section I Part 1](#p1)

Bezier Curves (.bzc)

[Section I Part 2](#p2)

Bezier Surfaces (.bez)

[All Parts in Section II and III](#s2)

COLLADA Mesh Files (.dae)

As an example, for Section I Part 1, you must load a Bezier curve as the following:

    ./meshedit ../bzc/curve1.bzc
    

Using the Graphical User Interface (GUI)
----------------------------------------

The following details how to use the GUI provided by the executable `meshedit`. You may want to skim through them on your first read-through; and read them in more details when you have implemented parts of this assignment and are ready to use the GUI.

### Section I Part 1

When you run `meshedit` with a Bezier curve (.bzc) for Section I Part I, you will see control points rendered on screen, as shown below.

![](https://i.imgur.com/wGCO3TI.png)

For this part **only**, the GUI is different. Below is the full specification on keyboard controls.

Key

Action

E

Perform one call to `BezierCurve::evaluateStep(...)` and cycle through levels

C

Toggle whether or not the fully evaluated Bezier curve is drawn to the screen

To verify your implementation is correct, you can repeatedly press E to cycle through each level of the de Casteljau subdivision. You can press C to toggle the Bezier curve to check if it is generated correctly based on the control points.

You can also use your mouse to:

*   **Click and drag** the control points to move them and see how your Bezier curve, along with all intermediate control points, changes accordingly.
*   **Scroll** to move the evaluated point along the Bezier curve and see how the intermediate control points move along with it. This is essentially varying ttt between 0.0 and 1.0.

### Section I Part 2 and Onwards

When you run `meshedit` from Section I Part 2 and onwards, you will see a triangle mesh rendered on screen, as shown below.

![](https://i.imgur.com/147nmya.png)

As you hover the cursor around the screen, you will notice that mesh elements (half-edges, vertices, edges,, and faces) under the cursor are highlighted in purple or white, such as an edge in the image above. You can click on one of these elements to select it and the GUI will display some information about the element and its associated data.

Below is the full specification on keyboard controls.

Key

Action

Q

Toggle using vertex normals (Part 3)

F

Flip the selected edge (Part 4)

S

Split the selected edge (Part 5)

L

Upsample the current mesh (Part 6)

N

Select the next half-edge

T

Select the twin half-edge

W

Toggle wireframe

I

Toggle information overlay

SPACE

Reset camera to default position

0 - 9

Switch between GLSL shaders

R

Recompile shaders

You can also use your mouse to:

*   **Click and drag a vertex** to change its position.
*   **Click and drag background** or **right click and drag anywhere** to rotate the camera.
*   **Scroll** to adjust the camera zoom.

You will implement area-weighted vertex normals (Q), local edge flip (F), local edge split (S), and loop subdivision (L) in [Part 3](#p3), [Part 4](#p4), [Part 5](#p5), and [Part 6](#p6), respectively. Therefore, these four key commands will do nothing until you have completed their respective part.

Starter Code Structure
----------------------

Before you start, here is some basic information on starter code structure.

For Bezier curves and surfaces ([Section I](#s1)), you will be filling in member functions of the `BezierCurve` and `BezierPatch` classes, defined in `bezierCurve.h` and `bezierPatch.h`.

For triangle meshes ([Section II](#s2)), you will be filling in member functions of the `Vertex`, `HalfedgeMesh`, and `MeshResampler` class, defined in `halfEdgeMesh.h`.

**We have put dummy definitions for all the functions you will need to modify within `student_code.cpp`. You will implement all parts of this assignment in this file!**

Section I: Bezier Curves and Surfaces
-------------------------------------

In computer graphics, Bezier curves and surfaces are frequently used to model smooth and infinitely scalable curves and surfaces, such as in [Adobe Illustrator](https://www.adobe.com/products/illustrator.html) (a curve drawing program) and in [Blender](https://www.blender.org/) (a surface modeling program).

A Bezier curve of degree nnn is defined by (n+1)(n + 1)(n+1) control points. It is a parametric curve based on a single parameter ttt, ranging between 0 and 1.

Similarly, a Bezier surface of degree (n,m)(n, m)(n,m) is defined by (n+1)×(m+1)(n + 1)\\times(m + 1)(n+1)×(m+1) control points. It is a parametric surface based on two parameters uuu and vvv, both ranging between 0 and 1.

In Section 1, you will use de Casteljau algorithm to evaluate Bezier curves and surfaces for any given set of control points and parameters, such as the Beizer curve below. In the image, white squares are the given control points, blue squares are the intermediate control points evaluated at the given parameter by de Casteljau algorithm, and the red square is the final evaluated point that lies on the Bezier curve.

![](http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_20.jpg)

### Part 1: Bezier Curves with 1D de Casteljau Subdivision (10 pts)

[**Relevant Lecture: 7**](https://cs184.eecs.berkeley.edu/sp23/lecture/7/)

In Part 1, you will implement Bezier curves. To get started, take a look at `bezierCurve.h` and examine the member variables defined in the class. Specifically, you will primarily be working with the following:

*   `std::vector<Vector2D> controlPoints`: A `std::vector` of original control points that define the Bezier curve, initialized from input Bezier curve file (`.bzc`).
*   `float t`: A parameter at which to evaluate the Bezier curve, ranging between 0 to 1.

Recall from lecture that de Casteljau algorithm gives us the recursive step below:

Given nnn (possibly intermediate) control points p1,...,pnp\_1, ..., p\_np​1​​,...,p​n​​ and the parameter ttt, we can use linear interpolation to compute the n−1n-1n−1 intermediate control points at the parameter ttt in the next subdivision level, p1′,...,pn−1′p\_1', ..., p\_{n-1}'p​1​′​​,...,p​n−1​′​​, where

pi′\=lerp(pi,pi+1,t)\=(1−t)pi+tpi+1p\_i' = lerp(p\_i, p\_{i+1}, t) = (1 - t)p\_i + tp\_{i+1} p​i​′​​\=lerp(p​i​​,p​i+1​​,t)\=(1−t)p​i​​+tp​i+1​​

By applying this step recursively, we eventually arrive at a final, single point and this point, in fact, lies **on** the Bezier curve at the given parameter ttt.

You need to implement this recursive step within `BezierCurve::evaluateStep(...)` in `student_code.cpp`. This function takes as input a `std::vector` of 2D points and outputs a `std::vector` of intermediate control points at the parameter ttt in the next subdivision level. Note that the parameter ttt is a member variable of the `BezierCurve` class, which you have access to within the function. **Each call to this function should performs only _one_ step of the algorithm, i.e., _one_ level of subdivision.**

#### Implementation Notes

`std::vector` is similar to Java's `ArrayList` class. You should use the `push_back(...)` method to add elements to a `std::vector`. This is analogous to the `append(...)` method of `ArrayList`. You can view this page for [more information on `push_back(...)`](https://en.cppreference.com/w/cpp/container/vector/push_back).

#### Sanity Check

To check if your implementation is likely correct, you can run `meshedit` with a Bezier curve file (`.bzc`) and view the generated Bezier curve on screen. Refer back here for [a list of controls **only** for Section I Part 1](#c1).

For example, you can run the following command:

    ./meshedit ../bzc/curve1.bzc
    

where `bzc/curve1.bzc` is a cubic Bezier curve. The provided `bzc/curve2.bzc` is a degree-4 Bezier curve. Feel free to create your own `.bzc` files and explore other Bezier curves.

#### Functions to Modify: Part 1

*   `BezierCurve::evaluateStep(...)`

#### For Your Write-Up: Part 1

*   Briefly explain de Casteljau's algorithm and how you implemented it in order to evaluate Bezier curves.
*   Take a look at the provided `.bzc` files and create your own Bezier curve with **6** control points of your choosing. Use this Bezier curve for your screenshots below.
*   Show screenshots of each step / level of the evaluation from the original control points down to the final evaluated point. Press E to step through. Toggle C to show the completed Bezier curve as well.
*   Show a screenshot of a slightly different Bezier curve by moving the original control points around and modifying the parameter ttt via mouse scrolling.

### Part 2: Bezier Surfaces with Separable 1D de Casteljau (15 pts)

[**Relevant Lecture: 7**](https://cs184.eecs.berkeley.edu/sp23/lecture/7/)

In Part 2, you will adapt what you have implemented for Bezier curves to Bezier surfaces. To get started, take a look at `bezierPatch.h` and examine the member variables defined in the class. Specifically, you will primarily be working with the following:

*   `std::vector<std::vector<Vector3D>> controlPoints`: A 2D `std::vector` that has n×nn \\times nn×n grid of original control points. `controlPoints` is of size nnn and each inner `std::vector`, i.e., `controlPoints[i]`, contains nnn control points at the iiith-row. For example, `controlPoints[0]` and `controlPoints[n-1]` each have nnn control points at the bottom row and the top row, respectively.

Recall from lecture that separable 1D de Casteljau algorithm works as the following:

The inputs to the algorithm are (1) a n×nn \\times nn×n grid of original control points, PijP\_{ij}P​ij​​, where iii and jjj are row and column index, and (2) the two parameters uuu and vvv.

We first consider that each **row** of nnn control points, Pi0,...,Pi(n−1)P\_{i0}, ..., P\_{i(n-1)}P​i0​​,...,P​i(n−1)​​, define a Bezier curve parameterized by uuu. Just as in Part 1, we can use the recursive step to evaluate the final, single point PiP\_iP​i​​ on this Bezier curve at the parameter uuu. We then consider that PiP\_iP​i​​ for all nnn rows define a Bezier curve parameterized by vvv. (These nnn points lie roughly along a column. This [lecture slide](https://cs184.eecs.berkeley.edu/sp19/lecture/7-90/geometry-and-splines) may help you visualize.) We can again use the recursive step from Part 1 to evaluate the final, single point PPP on this Bezier curve at the parameter vvv. This point PPP, in fact, lies **on** the Bezier surface at the given parameter uuu and vvv.

You need to implement the following functions in `student_code.cpp`:

*   `BezierPatch::evaluateStep(...)`: Very similar to `BezierCurve::evaluateStep(...)` in Part 1, this recursive function takes as inputs a `std::vector` of 3D points and a parameter ttt. It outputs a `std::vector` of intermediate control points at the parameter ttt in the next subdivision level.
*   `BezierPatch::evaluate1D(...)`: This function takes as inputs a `std::vector` of 3D points and a parameter ttt. It outputs directly **the final, single point** that lies on the Bezier curve at the parameter ttt. This function does **not** output intermediate control points. You may want to call `BezierPatch::evaluateStep(...)` inside this function.
*   `BezierPatch::evaluate(...)`: This function takes as inputs the parameter uuu and vvv and outputs the point that lies on the Bezier surface, defined by the n×nn \\times nn×n `controlPoints`, at the parameter uuu and vvv. Note that `controlPoints` is a member variable of the `BezierPatch` class, which you have access to within this function.

#### Sanity Check

To check if your implementation is likely correct, you can run `meshedit` with a Bezier surface file (.bez) and view the generated Bezier surface on screen.

For example, you can run the following command and you should see a teapot:

    ./meshedit ../bez/teapot.bez
    

#### Functions to Modify: Part 2

*   `BezierPatch::evaluateStep(...)`
*   `BezierPatch::evaluate1D(...)`
*   `BezierPatch::evaluate(...)`

#### For Your Write-Up: Part 2

*   Briefly explain how de Casteljau algorithm extends to Bezier surfaces and how you implemented it in order to evaluate Bezier surfaces.
*   Show a screenshot of `bez/teapot.bez` (**not** `.dae`) evaluated by your implementation.

Section II: Triangle Meshes and Half-Edge Data Structure
--------------------------------------------------------

**Important Note:**

*   In Section II, you will be working extensively with the half-edge data structure, as well as the provided `HalfedgeMesh` class, which implements the data structure.
*   Before diving into this section, we **recommend** that you read this [introduction to half-edge data strucutre](https://cs184.eecs.berkeley.edu/su20/docs/HalfEdgeIntro) to reinforce your understanding of it from [Lecture 8](https://cs184.eecs.berkeley.edu/sp23/lecture/8/meshes-and-geometry-processing).
*   We also **highly, highly recommend** that you carefully read through this primer on [how to navigate meshes using the `HalfedgeMesh` class](https://cs184.eecs.berkeley.edu/su20/docs/HalfEdgePrimer). The primer will help you get familiar with the provided `HalfedgeMesh` class quickly and present many examples, complete with code, that you may find very helpful for this section!
*   Finally, we encourage you to read and understand the documentation at the beginning of `halfedgeMesh.h`, which provides supplementary information to the primer.

In Section I, you have implemented Bezier surfaces, which are parametric surfaces defined by a 2D grid of control points. As discussed in lecture, we can also represent surfaces using [triangle meshes](https://cs184.eecs.berkeley.edu/sp20/lecture/8-1/meshes-and-geometry-processing). While Bezier surfaces are better at representing smooth surfaces than triangle meshes and require less memory, Bezier surfaces are much more difficult to render directly. (Can you reason why this is the case?) In fact, Bezier surfaces are often converted into triangle meshes before being rendered to screen.

As a result, triangle meshes are sometimes the preferred way to represent 3D geometric models in computer graphics. One way to store a triangle mesh is as a list of vertices and a list of triangles indexing the vertices, illustrated in this [lecture slide](https://cs184.eecs.berkeley.edu/sp20/lecture/8-13/meshes-and-geometry-processing). Unfortunately, such simple data structure does not allow for meaningful traversal of meshes required by many geometry processing tasks. For example, to find all triangles neighbouring a given triangle, we must iterate through the entire list of triangles, which can be prohibitively expensive if the mesh has [millions of triangles](https://cs184.eecs.berkeley.edu/sp20/lecture/8-2/meshes-and-geometry-processing).

To help answer the neighbouring triangle query and many others more efficiently, the [half-edge data structure](https://cs184.eecs.berkeley.edu/sp20/lecture/8-22/meshes-and-geometry-processing) explicitly stores the connectivity information among mesh elements, i.e., vertices, edges, and faces. Throughout this section, you will use the half-edge data structure to implement a few commonly used geometric operations; and we hope you will get to appreciate the simplicity and flexibility of this data structure.

### Part 3: Area-Weighted Vertex Normals (10 pts)

[**Relevant Lecture: 8**](https://cs184.eecs.berkeley.edu/sp23/lecture/8/)

In Part 3, you will implement area-weighted normal vectors at vertices. Recall from [lecture](https://cs184.eecs.berkeley.edu/sp19/lecture/6-31/the-rasterization-pipeline) that these vertex normals can be used for [Phong shading](https://upload.wikimedia.org/wikipedia/commons/e/ec/Phongshading01.png), which provides better shading for smooth surfaces than [flat shading](https://upload.wikimedia.org/wikipedia/commons/3/3c/Flatshading01.png) (image credit: Wikipedia).

To get started, take a look at the `Vertex` class defined in `halfedgeMesh.h`, along with the very helpful comments within the class. In short, a `Vertex` object encapsulates a single mesh vertex and has member variables such as the following (the list is **not** exhaustive):

*   `Vector3D position`: The 3D coordinate of this vertex.
*   `HalfedgeIter& halfedge`: A reference to the half-edge rooted at this vertex.
*   `HalfedgeCIter halfedge`: The same half-edge as above, except this variable is `const` (i.e., constant) and cannot be modified.

To compute an area-weighted normal at a given vertex, you should use the half-edge data structure to iterate through faces (triangles) incident to the vertex, i.e., faces that have the given vertex as one of its vertices. For each such face, you weight its normal by its area. Finally, you normalize the sum of all area-weighted normals.

You need to implement this part in `Vertex::normal()` in `student_code.cpp`. This function takes no input and outputs the area-weighted vertex normal. Since `normal()` is a member function of the `Vertex` class, you have access to the class member variables, such as `Vector3D position`, within the function. To understand how to iterate through faces incident to a vertex, you may find the example `printNeighborPositions(...)` in this [primer](https://cs184.eecs.berkeley.edu/sp20/article/18/a-primer-on-the-halfedgemesh-cla) helpful. (Hint: A face in the [half-edge data structure](https://cs184.eecs.berkeley.edu/sp20/lecture/8-22/meshes-and-geometry-processing) only points to its associated half-edge. To compute the area and normal of a face, what you really need are its three vertices instead of the face itself. A normal of a face is defined as a vector perpendicular to the surface at a given point. The cross product of two vectors along the face would return a third vector perpendicular to the two vectors.)

#### Implementation Notes

For Part 3 only, you are implementing a `const` (i.e., constant) member function, which requires that no code within this function modifies any member variables. This just means that you should use `HalfedgeCIter` and not `HalfedgeIter` in `Vertex::normal()`.

For more information on the differences between `Halfedge *`, `HalfedgeIter`, and `HalfedgeCIter`, check out this short article on [Iterators vs Pointers](https://cs184.eecs.berkeley.edu/su20/docs/IteratorsVsPointers).

You may also find this [CGL Vector Documentation](https://cs184.eecs.berkeley.edu/sp23/docs/cgl-vector-docs) helpful for implementing any operations between vectors.

#### Sanity Check

To check if your implementation is likely correct, you can run `meshedit` with a COLLADA mesh file (`.dae`). Once the model is loaded, press Q to toggle the area-averaged normals, which calls `Vertex::normal()` you have implemented, and the shading of the model should become [smoother](https://upload.wikimedia.org/wikipedia/commons/e/ec/Phongshading01.png) and no longer [flat](https://upload.wikimedia.org/wikipedia/commons/3/3c/Flatshading01.png). Refer back here for [a list of controls for Section I Part 2 and onwards](#c2).

For example, you can run the following command:

    ./meshedit ../dae/teapot.dae
    

After you press Q, the shading of the teapot should look smooth like the image below. If your shading differs from the image, your implementation of `Vertex::normal()` is likely incorrect. One common mistake is that the computed normals point towards the interior of the model, as opposed to the exterior required by computer graphics. The oppositely oriented normals lead to very dark shading, which can be fixed by reversing the normals.

![](http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_18.jpg)

#### Functions to Modify: Part 3

*   `Vertex::normal()`

#### For Your Writeup: Part 3

*   Briefly explain how you implemented the area-weighted vertex normals.
*   Show screenshots of `dae/teapot.dae` (**not** `.bez`) comparing teapot shading with and without vertex normals. Use Q to toggle default flat shading and Phong shading.

### Part 4: Edge Flip (15 pts)

[**Relevant Lecture: 8**](https://cs184.eecs.berkeley.edu/sp23/lecture/8/)

In Part 4, you will implement a local remeshing operation on an edge, called a **flip**. Given a pair of triangles (a,b,c)(a,b,c)(a,b,c) and (c,b,d)(c,b,d)(c,b,d), a flip operation on their shared edge (b,c)(b,c)(b,c) converts the original pair of triangles into a new pair (a,d,c)(a,d,c)(a,d,c) and (a,b,d)(a,b,d)(a,b,d), as shown below:

![](http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_9.jpg)

You need to implement this part in `HalfedgeMesh::flipEdge(...)` in `student_code.cpp`. This function takes as input an `EdgeIter` to an edge that needs to be flipped, e.g., edge (b,c)(b,c)(b,c) above, and outputs an `EdgeIter` to the now flipped edge, e.g., edge (a,d)(a,d)(a,d) above.

To properly implement any remeshing operations, you need to take extra care to make sure that, in the modified mesh, all pointers of all mesh elements still point to the right elements. A mesh element is a half-edge, vertex, edge, or face. Here is a [refresher](https://cs184.eecs.berkeley.edu/sp20/lecture/8-22/meshes-and-geometry-processing) on what each mesh element points to. We **recommend** that you perform the following steps to ensure that all pointers of all elements are still valid after remeshing operations, such as edge flips in this part or edge splits in the next part:

1.  Draw a simple mesh, such as the pair of triangles (a,b,c)(a,b,c)(a,b,c) and (c,b,d)(c,b,d)(c,b,d) above, and write down a list of all elements, i.e., half-edges, vertices, edges, and faces, in this mesh.
2.  Draw the mesh after the remeshing operation and again write down a list of all elements in the now modified mesh.
3.  If the remeshing operation adds new elements in the modified mesh, create these new elements. An edge flip **does not** add new elements, but an edge split in Part 5 **does**.
4.  For **every** element in the modified mesh, set **all** of its pointers to the correct element in the modified mesh, **even if** the element being pointed to has not changed:
    *   For each vertex, edge, and face, set its `halfedge` pointer.
    *   For each half-edge, set its `next`, `twin`, `vertex`, `edge`, and `face` pointer to the correct element. You can use `Halfedge::setNeighbors(...)` to set all pointers of a half-edge at once.
    *   We recommend setting all pointers of all elements in the modified mesh, not just the ones that have changed, because it is very easy to miss a pointer and get errors that are very difficult to debug. Once you are sure that your implementation works, you can remove the unnecessary pointer assignments if you wish.

Your implementation of `HalfedgeMesh::flipEdge(...)` should do the following:

*   Never flip a boundary edge. The function should simply return immediately if either neighbouring face of the edge is on a boundary loop. Every mesh element has a useful `isBoundary()` function that returns true if the element is on the boundary.
*   Perform only a constant amount of work. The cost of flipping a single edge should not be proportional to the mesh size.
*   Do **not** add or delete any mesh elements. There should be exactly the same number of elements before and after the edge flip. You only need to reassign pointers.

#### Implementation Notes

Given any mesh element, you can use the provided `check_for(...)` debugging function to check which other elements in the mesh point to it. For example, you can use this function to confirm if an element is pointed to by the right number of other elements. Please read the Debugging Aid section in this [primer on the `HalfedgeMesh` class](https://cs184.eecs.berkeley.edu/su20/docs/HalfEdgePrimer) for more information.

#### Sanity Check

After loading a model from a `.dae` file, you can click to select an edge and press F to flip it. Refer back here for [a list of controls for Section I Part 2 and onwards](#c2). Sometimes, your implementation may seem to work when you flip an edge only once. We recommend that you flip an edge **a few more times** to be more certain that your code really works as expected. Part 6 will require you to have a working implementation of edge flips. There should be no holes in your mesh created by an edge flip. Note that you may reach a degenerate case where one triangle looks much darker than the others, this is ok.

#### Functions to Modify: Part 4

*   `HalfedgeMesh::flipEdge(...)`

#### For Your Writeup: Part 4

*   Briefly explain how you implemented the edge flip operation and describe any interesting implementation / debugging tricks you have used.
*   Show screenshots of the teapot before and after some edge flips.
*   Write about your eventful debugging journey, if you have experienced one.

### Part 5: Edge Split (15 pts)

[**Relevant Lectures: 8**](https://cs184.eecs.berkeley.edu/sp23/lecture/8/)

In Part 5, you will implement a different local remeshing operation on an edge, called a **split**. Given a pair of triangles (a,b,c)(a,b,c)(a,b,c) and (c,b,d)(c,b,d)(c,b,d), a split operation on their shared edge (b,c)(b,c)(b,c) inserts a new vertex mmm at its midpoint and connects the new vertex to each opposing vertex aaa and ddd, yielding four triangles as shown below:

![](http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_10.jpg)

You need to implement this part in `HalfedgeMesh::splitEdge(...)` in `student_code.cpp`. This function takes as input an `EdgeIter` to an edge that needs to be split, e.g., edge (b,c)(b,c)(b,c) above, and outputs a `VertexIter` to the newly inserted vertex, e.g., vertex mmm above.

Because an edge split adds new mesh elements, e.g., a new vertex, two new triangles, three new edges, and etc, you will have more pointers to keep track of and may find that an edge split is a little trickier to implement than an edge flip. We encourage you to again follow [our recommendation in Part 4](#rec) to ensure that all pointers of all mesh elements point to the right elements in the modified mesh.

Your implementation of `HalfedgeMesh::splitEdge(...)` should do the following:

*   Ignore requests to split boundary edges, unless you are trying for extra credit. The function can simply return immediately if either neighbouring face of the edge is on a boundary loop. Note that splitting a boundary edge **does** make sense, but flipping a boundary edge **does not** make sense. (Can you reason why this is the case?)
*   Assign the position of the new vertex to the midpoint of the original edge. Remember from [Part 3](#p3) that the `Vertex` class has a member variable `Vector3D position`.
*   Perform only a constant amount of work. The cost of splitting a single edge should not be proportional to the mesh size.
*   Create only as many new elements as needed. The mesh should **not** have any elements that is not connected to the rest of the mesh.

**Extra Credit:** _Support edge splits for boundary edges._ For this, you will need to carefully read the paragraphs on virtual boundary face right before the Iterating Over Mesh Entities section in this [primer on the `HalfedgeMesh` class](https://cs184.eecs.berkeley.edu/su20/docs/HalfEdgePrimer). In short, you will split the edge in half, but only split in half the face that is non-boundary.

#### Sanity Check

After loading a model from a `.dae` file, you can click to select an edge and press S to split it. Refer back here for [a list of controls for Section I Part 2 and onwards](#c2). To verify that your implementation is likely correct, you can flip some edges that you have split and then split some edges that you have flipped. You can also alternate between flipping and splitting edges many times in mesh regions that are nearby and far apart; and check if the mesh changes correctly. Part 6 will require you to have a working implementation of edge splits.

#### Functions to Modify: Part 5

*   `HalfedgeMesh::splitEdge(...)`

#### For Your Writeup: Part 5

*   Briefly explain how you implemented the edge split operation and describe any interesting implementation / debugging tricks you have used.
*   Show screenshots of a mesh before and after some edge splits.
*   Show screenshots of a mesh before and after a combination of both edge splits and edge flips.
*   Write about your eventful debugging journey, if you have experienced one.
*   If you have implemented support for boundary edges, show screenshots of your implementation properly handling split operations on boundary edges.

### Part 6: Loop Subdivision for Mesh Upsampling (25 pts)

[**Relevant Lecture: 8**](https://cs184.eecs.berkeley.edu/sp23/lecture/8/)

Sometimes, we may wish to convert a coarse polygon mesh into a higher-resolution one for better display, more accurate simulation, and etc. Such conversion requires an upsampling algorithm that nicely interpolates or approximates the original mesh data.

Unfortunately, the techniques we have learned to upsample 2D images, such as bilinear filtering in [Lecture 5](https://cs184.eecs.berkeley.edu/sp20/lecture/5-45/texture-mapping), do not easily translate to upsampling 3D meshes. Among many reasons, a mesh often has vertices at **irregular** locations, as opposed to on a regular grid like an image. (Can you think of other reasons?)

In Part 6, you will implement a mesh upsampling method, called **loop subdivision**. In short, loop subdivision upsamples a mesh by (1) subdividing each of its triangles into four smaller triangles and (2) updating vertices of subdivided mesh based on some weighting scheme.

You need to implement these two steps of loop subdivision, outlined in more details below, in `MeshResampler::upsample(...)` in `student_code.cpp`. This function has no outputs and takes as input a reference to a `HalfedgeMesh` that will be subdivided.

A single loop subdivision consists of the following two steps. If we repeatedly apply these two steps, we will converge to a smooth approximation of our original mesh.

1.  Subdivide each triangle in the mesh into four by connecting edge midpoints, as shown below. This is called a **4-1 subdivision**. ![](http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_11.jpg) To perform 4-1 subdivisions over the entire mesh, you can use the edge flip and edge split operation you have implemented. Specifically, you can do the following: ![](http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_14.jpg) (1) Split every existing edge of the mesh in any order. (2) Flip any new edge that connects an old vertex and a new vertex.
    
    **Important Note:** After (1), every original edge will now be represented by 2 edges. You should **not** flip these edges because they are already along the boundary of the 4-way subdivided triangles. In the figure above, you should **only** flip blue edges that connect an old vertex and new vertex. You should **not** flip any black new edges.
    
2.  Update vertex positions as weighted average of neighboring vertex positions. Each new vertex position can be calculated from the original vertex positions as seen in the figure below.
    
    The figure below shows the weights we use to (1) compute the position of a newly added vertex or to (2) update the position of an existing vertex. The new vertex and old vertex are depicted as the white circle in their respective mesh.
    
    ![](http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_13.jpg)
    
    (1) The position of a new vertex splitting the shared edge (A,B)(A, B)(A,B) between a pair of triangle (A,C,B)(A, C, B)(A,C,B) and (A,B,D)(A, B, D)(A,B,D) is
    
        3/8 * (A + B) + 1/8 * (C + D)
        
    
    (2) The updated position of an old vertex is
    
        (1 - n * u) * original_position + u * original_neighbor_position_sum
        
    
    where `n` is the vertex degree, i.e., number of edges incident to the vertex, `u` is the constant shown in the figure, `original_position` is the **original** position of the old vertex, and `original_neighbor_position_sum` is the sum of all **original** positions of the neighboring vertices. Look here for [an example of degree-6 vertex](https://cs184.eecs.berkeley.edu/sp20/lecture/8-37/meshes-and-geometry-processing).
    

#### Implementation Notes

While you can implement loop subdivision exactly as described, we actually **recommend** that you update vertex positions **before** performing 4-1 subdivisions over the entire mesh. More specifically, you may want to follow the steps below:

*   Step A: Compute the positions of both new and old vertices using the original mesh. We want to perform these computations before subdivision because traversing a coarse mesh is much easier than traversing a subdivided mesh with more elements.
*   Step B: Subdivide the original mesh via edge splits and flips as described.
*   Step C: Update all vertex positions in the subdivided mesh using the values already computed.

If you choose to follow our recommendation, you may consider using the following member variables of the `Vertex` and `Edge` class to facilitate your implementation:

*   `Vector3D Vertex::newPosition` temporarily stores the updated position of a new or old vertex, computed using the weighted average described above.
*   `Vector3D Edge::newPosition` temporarily stores the position of the vertex that will ultimately be inserted at the edge midpoint.
*   `bool Vertex::isNew` flags whether a vertex exists in the original mesh or is a new vertex inserted at an edge midpoint by subdivision.
*   `bool Edge::isNew` flags whether an edge exists in the original mesh or is a new edge added by subdivision.

**Important Note:** While computing the updated vertex positions in Step A, you should **not** change the value of `Vector3D Vertex::position` because the weighted average is based on vertex positions in the **original** mesh. (Please take a moment to understand why.) You will, however, need to update `Vertex::position` in Step C.

Examples below illustrate the correct behavior of the loop subdivision algorithm:

![](http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_12.jpg)

**Extra Credit:** _Support meshes with boundary._ You will first need to make sure that your edge split function appropriately handles boundary edges. You do not need to change your edge flip function. You will also need a different weighted average for boundary vertices. See ["A Survey of Subdivision-Based Tools for Surface Modeling"](http://mrl.nyu.edu/~dzorin/papers/boiermartin2005sbt.pdf) for more information.

**Extra Credit:** _Implement additional subdivision schemes._ There exist many alternatives to loop subdivision. Triangle subdivision schemes include the [Butterfly scheme](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.133.8925&rep=rep1&type=pdf), the [modified Butterfly scheme](http://mrl.nyu.edu/~dzorin/papers/zorin1996ism.pdf), and [3\\sqrt{3}√​3​​​\-Subdivision](https://www.graphics.rwth-aachen.de/media/papers/sqrt31.pdf). One of the most popular subdivision schemes for quadrilateral meshes is [Catmull-Clark](https://en.wikipedia.org/wiki/Catmull%E2%80%93Clark_subdivision_surface). There are also special subdivision schemes, such as [polar subdivision](http://www.cise.ufl.edu/research/SurfLab/papers/09bi3c2polar.pdf) for handling meshes with vertices of high degree.

#### Functions to Modify: Part 6

*   `MeshResampler::upsample(...)`

#### For Your Writeup: Part 6

*   Briefly explain how you implemented the loop subdivision and describe any interesting implementation / debugging tricks you have used.
*   Take some notes, as well as some screenshots, of your observations on how meshes behave after loop subdivision. What happens to sharp corners and edges? Can you reduce this effect by pre-splitting some edges?
*   Load `dae/cube.dae`. Perform several iterations of loop subdivision on the cube. Notice that the cube becomes slightly asymmetric after repeated subdivisions. Can you pre-process the cube with edge flips and splits so that the cube subdivides symmetrically? Document these effects and explain why they occur. Also explain how your pre-processing helps alleviate the effects.
*   If you have implemented any extra credit extensions, explain what you did and document how they work with screenshots.

Section III: Potential Extra Credit - Art Competition: Model something Creative!
--------------------------------------------------------------------------------

You can design your own polygon mesh using the free program [Blender](http://blender.org) and save it as a COLLADA mesh file (`.dae`). For a baseline starting point, we recommend designing a humanoid mesh with a head, two arms, and two legs. We have created a [video tutorial](https://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/blender.mp4) and an [article](https://cs184.eecs.berkeley.edu/sp20/article/14/blender-modeling) on Blender to guide you through the basics of making a simple humanoid mesh.

Once you have designed your mesh and saved it as a `.dae` file, you should load it into the `meshedit` program and subdivide it to smooth it out. The figure below shows the simple humanoid mesh before and after subdivisions, as well as with a shader of environment map reflection applied.

![](http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_15.jpg)

Here are a few other ways you can express your creativity:

*   Add additional detail to your mesh - fingers, facial features.
*   Google "box modeling" (images and videos) to get inspired.
*   Investigate additional functionality in Blender to design alternative shapes.
*   Programmatically generate a mesh that approximates a 3D fractal or other complex shape.
*   Write a super cool custom shader that makes your mesh look awesome!
*   Implement additional geometric operations and demonstrate them on your mesh.

#### For Your Art Competition Submission (Optional, Possible Extra Credit)

*   You do not need to have your art ready at the time of submission. The Art Competition will open the following day after the Assignment 2 deadline.
*   If submitting at the time of the project submission, save your best polygon mesh as `partsevenmodel.dae` in your `docs` folder
*   Show us a screenshot of the mesh when submitting your art in the competition Google Form.
*   Include a series of screenshots showing your original mesh and your mesh after one and two rounds of subdivision. If you have used custom shaders, include screenshots of your mesh with those shaders applied as well.
*   Describe what you have done to enhance your mesh beyond the simple humanoid mesh described in the tutorial.

Friendly Advices From Your GSIs
-------------------------------

*   Start early. This assignment has multiple parts and you want to manage your time accordingly. Remember that pesky bugs can waste your time like no other; and always keep in mind [Hofstadter's Law](https://en.wikipedia.org/wiki/Hofstadter's_law).
*   Part 1 and 2 should be relatively straightforward to implement once you understand Bezier curves and surfaces and de Casteljau algorithm.
*   Part 3 to 6 will be more difficult to implement correctly right away since they involve managing pointers, which are exposed to you as iterators. Make sure you test these parts, especially Part 4 and 5, **together** on a few different meshes. While correct behaviors do not imply correct code, incorrect behaviors do imply incorrect code.
*   The optional Section III for possible extra credit involves learning how to use Blender, which takes time even with video tutorials. We recommend starting early, in case you have questions. Learning Blender does not depend on any other part of this assignment, so you can do it anytime, perhaps as a nice break from chasing pointers, :).
*   Make sure you allocate enough time to do a good job on the write-up!

Submission
----------

Please consult this article on [how to submit assignments for CS184](https://cs184.eecs.berkeley.edu/sp23/docs/submitting-assignments). You will submit your code and some deliverables (see below) in a webpage project write-up.

### Project Write-Up Guidelines and Instructions

We have provided a simple HTML skeleton in `index.html` found within the `docs` folder to help you get started and structure your write-up.

You are also welcome to create your own webpage report from scratch using your own preferred frameworks or tools. However, **please follow the same overall structure as described in the deliverables section below**.

The goals of your write-up are for you to (1) think about and articulate what you have built and learned in your own words and (2) have a write-up of the project to take away from the class. Your write-up should include the following:

*   An overview of the project, including your approach to and implementation for each of the parts, as well as what problems you have encountered and how you solved them. Strive for clarity and succinctness.
*   For each part, make sure to include the results described in the corresponding [Deliverables](#rubric) section, in addition to your explanation. If you failed to generate any results correctly, provide a brief explanation on why.
*   The final, optional part seven, you have the opportunity to be creative and individual! Be sure to provide a good description on what you were going for, what you did, and how you did it, :).
*   Clearly indicate any extra credit items you have completed; and provide a thorough explanation and illustration for each of them.

The write-up is one of our main methods to evaluate your work, so it is important to spend the time to do it correctly and thoroughly. Plan ahead to allocate time for the write-up well before the deadline.

### Project Write-Up Deliverables and Rubric

This rubric lists the basic, minimum requirements for your write-up. The content and quality of your write-up are extremely important. You should make sure to at least address all the points listed below. The extra credits are intended for students who want to challenge themselves and explore methods beyond the fundamentals. They are not worth a lot of points, so do not necessarily expect to use extra credit points to make up for lost points elsewhere.

#### Overview

Give a high-level overview of what you have implemented in this assignment. Think about what you have built as a whole. Share your thoughts on what interesting things you have learned from completing this assignment.

#### Part 1

*   Briefly explain de Casteljau's algorithm and how you implemented it in order to evaluate Bezier curves.
*   Take a look at the provided `.bzc` files and create your own Bezier curve with **6** control points of your choosing. Use this Bezier curve for your screenshots below.
*   Show screenshots of each step / level of the evaluation from the original control points down to the final evaluated point. Press E to step through. Toggle C to show the completed Bezier curve as well.
*   Show a screenshot of a slightly different Bezier curve by moving the original control points around and modifying the parameter ttt via mouse scrolling.

#### Part 2

*   Briefly explain how de Casteljau algorithm extends to Bezier surfaces and how you implemented it in order to evaluate Bezier surfaces.
*   Show a screenshot of `bez/teapot.bez` (**not** `.dae`) evaluated by your implementation.

#### Part 3

*   Briefly explain how you implemented the area-weighted vertex normals.
*   Show screenshots of `dae/teapot.dae` (**not** `.bez`) comparing teapot shading with and without vertex normals. Use Q to toggle default flat shading and Phong shading.

#### Part 4

*   Briefly explain how you implemented the edge flip operation and describe any interesting implementation / debugging tricks you have used.
*   Show screenshots of a mesh before and after some edge flips.
*   Write about your eventful debugging journey, if you have experienced one.

#### Part 5

*   Briefly explain how you implemented the edge split operation and describe any interesting implementation / debugging tricks you have used.
*   Show screenshots of a mesh before and after some edge splits.
*   Show screenshots of a mesh before and after a combination of both edge splits and edge flips.
*   Write about your eventful debugging journey, if you have experienced one.
*   If you have implemented support for boundary edges, show screenshots of your implementation properly handling split operations on boundary edges.

#### Part 6

*   Briefly explain how you implemented the loop subdivision and describe any interesting implementation / debugging tricks you have used.
*   Take some notes, as well as some screenshots, of your observations on how meshes behave after loop subdivision. What happens to sharp corners and edges? Can you reduce this effect by pre-splitting some edges?
*   Load `dae/cube.dae`. Perform several iterations of loop subdivision on the cube. Notice that the cube becomes slightly asymmetric after repeated subdivisions. Can you pre-process the cube with edge flips and splits so that the cube subdivides symmetrically? Document these effects and explain why they occur. Also explain how your pre-processing helps alleviate the effects.
*   If you have implemented any extra credit extensions, explain what you did and document how they work with screenshots.

#### Potential Extra Credit - Art Competition: Model something Creative!

*   You do not need to have your art ready at the time of submission. The Art Competition will open the following day after the Assignment 2 deadline.
*   If submitting at the time of the project submission, save your best polygon mesh as `artcompetition.dae` in your `docs` folder
*   Include a series of screenshots showing your original mesh and your mesh after one and two rounds of subdivision. If you have used custom shaders, include screenshots of your mesh with those shaders applied as well.
*   Describe what you have done to enhance your mesh beyond the simple humanoid mesh described in the tutorial.

### Website Tips and Advice

*   Your main report page should be called `index.html`.
*   Be sure to include and turn in all of the other files, such as images, that are linked in your report!
*   Use only **relative** paths to files, such as `"./images/image.jpg"`.
*   Do **not** use absolute paths, such as `"/Users/student/Desktop/image.jpg"`.
*   Pay close attention to your file extensions. Remember that on UNIX systems, such as the instructional machines, capitalization matters (`.png != .jpeg != .jpg != .JPG`).
*   Be sure to adjust the permissions on your files so that they are world readable. For more information, please see this [tutorial](https://www.grymoire.com/Unix/Permissions.html).
*   Start assembling your webpage early to make sure you have a handle on how to edit the HTML code to insert images and format sections.

$\_mod\_cal184.ready();(function(){var w=window;w.$components=(w.$components||\[\]).concat({"r":"M","w":\[\["s0-0-0-3-10",0,{"to":"/comments","l":"comments"},{"f":1,"s":{"active":false},"w":{"target":"/sp23/comments"}}\],\["s0-0-0-3-9",0,{"to":"/resources","l":"resources"},{"f":1,"s":{"active":false},"w":{"target":"/sp23/resources"}}\],\["s0-0-0-3-8",0,{"to":"/readings","l":"readings"},{"f":1,"s":{"active":false},"w":{"target":"/sp23/readings"}}\],\["s0-0-0-3-7",0,{"to":"/staff","l":"staff"},{"f":1,"s":{"active":false},"w":{"target":"/sp23/staff"}}\],\["s0-0-0-3-6",0,{"to":"/policies","l":"policies"},{"f":1,"s":{"active":false},"w":{"target":"/sp23/policies"}}\],\["s0-0-0-3-5",0,{"to":"/","l":"cs184/284a"},{"f":1,"s":{"active":false},"w":{"target":"/sp23"}}\]\],"t":\["/cal-184-website$3.2.0/client/components/comp-nav-link/index.marko"\]})||w.$components})()
