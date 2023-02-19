#include "rasterizer.h"

using namespace std;

namespace CGL {

  RasterizerImp::RasterizerImp(PixelSampleMethod psm, LevelSampleMethod lsm,
    size_t width, size_t height,
    unsigned int sample_rate) {
    this->psm = psm;
    this->lsm = lsm;
    this->width = width;
    this->height = height;
    this->sample_rate = sample_rate;

    sample_buffer.resize(width * height * sample_rate, Color::White);
  }

  // Used by rasterize_point and rasterize_line
  void RasterizerImp::fill_pixel(size_t x, size_t y, Color c) {
    // TODO: Task 2: You might need to this function to fix points and lines (such as the black rectangle border in test4.svg)
    // NOTE: You are not required to implement proper supersampling for points and lines
    // It is sufficient to use the same color for all supersamples of a pixel for points and lines (not triangles)
    //fill line and points at the sample_buffer scale
    float scale = sqrt(sample_rate);
    int x_sub = x * sqrt(sample_rate);
    int y_sub = y * sqrt(sample_rate);

    for (int xa = x_sub; xa < (x_sub + sqrt(sample_rate)); ++xa) {
        for (int ya = y_sub; ya < (y_sub + sqrt(sample_rate)); ++ya) {
            sample_buffer[ya * width*scale + xa] = c;
        }
    }
  }

  // Rasterize a point: simple example to help you start familiarizing
  // yourself with the starter code.
  //
  void RasterizerImp::rasterize_point(float x, float y, Color color) {
    // fill in the nearest pixel
    int sx = (int)floor(x);
    int sy = (int)floor(y);

    // check bounds
    if (sx < 0 || sx >= width) return;
    if (sy < 0 || sy >= height) return;

    fill_pixel(sx, sy, color);
    return;
  }

  // Rasterize a line.
  void RasterizerImp::rasterize_line(float x0, float y0,
    float x1, float y1,
    Color color) {
    if (x0 > x1) {
      swap(x0, x1); swap(y0, y1);
    }

    float pt[] = { x0,y0 };
    float m = (y1 - y0) / (x1 - x0);
    float dpt[] = { 1,m };
    int steep = abs(m) > 1;
    if (steep) {
      dpt[0] = x1 == x0 ? 0 : 1 / abs(m);
      dpt[1] = x1 == x0 ? (y1 - y0) / abs(y1 - y0) : m / abs(m);
    }

    while (floor(pt[0]) <= floor(x1) && abs(pt[1] - y0) <= abs(y1 - y0)) {
      rasterize_point(pt[0], pt[1], color);
      pt[0] += dpt[0]; pt[1] += dpt[1];
    }
  }

  //To give the score if a point(x,y) past the line equation test
  float inside_test(float x, float y, float x0, float y0, float x1, float y1) {
      float inside_score = -(x - x0) * (y1 - y0) + (y - y0) * (x1 - x0);
      return inside_score;
  }

  bool insidetriangle(float x0, float y0, float x1, float y1, float x2, float y2, int x, int y) {
      if ((inside_test(x, y, x0, y0, x1, y1) * inside_test(x2, y2, x0, y0, x1, y1)) >= 0 &&
          (inside_test(x, y, x0, y0, x2, y2) * inside_test(x1, y1, x0, y0, x2, y2)) >= 0 &&
          (inside_test(x, y, x2, y2, x1, y1) * inside_test(x0, y0, x2, y2, x1, y1)) >= 0) {
          return true;
      }
      else {
          return false;
      }
  }


  // Rasterize a triangle.
  void RasterizerImp::rasterize_triangle(float x0, float y0,
    float x1, float y1,
    float x2, float y2,
    Color color) {
    // TODO: Task 1: Implement basic triangle rasterization here, no supersampling

    //Determine the rectangular box to figure out the range of point to loop through
    int sample_rate = this->sample_rate;
    float xmax = max(x0, max(x1, x2));
    float ymax = max(y0, max(y1, y2));
    float xmin = min(x0, min(x1, x2));
    float ymin = min(y0, min(y1, y2));
    float edgenum = sqrt(sample_rate);
    //for every point within the box
    /*for (int x = xmin; x < xmax; ++x) {
      for (int y = ymin; y < ymax; ++y) {
        float xtest = x + 0.5;
        float ytest = y + 0.5;
        if (insidetriangle(x0, y0, x1, y1, x2, y2, xtest, ytest)) {
          rasterize_point(x, y, color);
        }
      }
    }*/
   
    //// TODO: Task 2: Update to implement super-sampled rasterization
    
    //initiate a sample buffer for super sampling based on sample rate
    //scale the vertices to the super sampling
    float x0_scale = x0 * edgenum, y0_scale = y0 * edgenum,
        x1_scale = x1 * edgenum, y1_scale = y1 * edgenum,
        x2_scale = x2 * edgenum, y2_scale = y2 * edgenum;
    float xmax_scale = max(x0_scale, max(x1_scale, x2_scale));
    float ymax_scale = max(y0_scale, max(y1_scale, y2_scale));
    float xmin_scale = min(x0_scale, min(x1_scale, x2_scale));
    float ymin_scale = min(y0_scale, min(y1_scale, y2_scale));
    size_t width_scale = width * edgenum;
    size_t height_scale = height * edgenum;
    for (int x = xmin_scale; x < xmax_scale; ++x) {
        int sx = (int)floor(x);
        if (sx < 0 || sx >= width_scale) break;
        for (int y = ymin_scale; y < ymax_scale; ++y) {
            int sy = (int)floor(y);
            if (y < 0 || y >= height_scale) break;
            float xtest = x + 0.5;
            float ytest = y + 0.5;
            if (insidetriangle(x0_scale, y0_scale, x1_scale, y1_scale, x2_scale, y2_scale, xtest, ytest)) {
                sample_buffer[sy * width_scale + sx] = color ;
            }
        }
    }
    

  }
  // use this to find weight for Barycentric coordinates
  float finda(float x0, float y0, float x1, float y1, float x2, float y2, float x, float y) {
      float a = (-(x - x1) * (y2 - y1) + (y - y1) * (x2 - x1)) / (-(x0 - x1) * (y2 - y1) + (y0 - y1) * (x2 - x1));
      return a;
  }

  void RasterizerImp::rasterize_interpolated_color_triangle(float x0, float y0, Color c0,
    float x1, float y1, Color c1,
    float x2, float y2, Color c2)
  {
    // TODO: Task 4: Rasterize the triangle, calculating barycentric coordinates and using them to interpolate vertex colors across the triangle
    // Hint: You can reuse code from rasterize_triangle
    // Draw the inside of the triangles
    //Determine the rectangular box to figure out the range of point to loop through
    float edgenum = sqrt(this->sample_rate);
    float x0_scale = x0 * edgenum, y0_scale = y0 * edgenum,
        x1_scale = x1 * edgenum, y1_scale = y1 * edgenum,
        x2_scale = x2 * edgenum, y2_scale = y2 * edgenum;
    float xmax_scale = max(x0_scale, max(x1_scale, x2_scale));
    float ymax_scale = max(y0_scale, max(y1_scale, y2_scale));
    float xmin_scale = min(x0_scale, min(x1_scale, x2_scale));
    float ymin_scale = min(y0_scale, min(y1_scale, y2_scale));
    size_t width_scale = width * edgenum;
    size_t height_scale = height * edgenum;
    for (int x = xmin_scale; x < xmax_scale; ++x) {
      int sx = (int)floor(x);
      if (sx < 0 || sx >= width_scale) break;
      for (int y = ymin_scale; y < ymax_scale; ++y) {
        int sy = (int)floor(y);
        if (y < 0 || y >= height_scale) break;
        float xtest = x + 0.5;
        float ytest = y + 0.5;
        if (insidetriangle(x0_scale, y0_scale, x1_scale, y1_scale, x2_scale, y2_scale, xtest, ytest)) {
            
          float alpha = finda(x0_scale, y0_scale, x1_scale, y1_scale, x2_scale, y2_scale, x, y);
          float beta = finda(x1_scale, y1_scale, x2_scale, y2_scale, x0_scale, y0_scale, x, y);
          float gamma = 1.0 - alpha - beta;
          Color color = (alpha * c0 + beta * c1 + gamma * c2);
          sample_buffer[sy * width_scale + sx] = color;
        }
      }
    }
   
  }


  void RasterizerImp::rasterize_textured_triangle(float x0, float y0, float u0, float v0,
    float x1, float y1, float u1, float v1,
    float x2, float y2, float u2, float v2,
    Texture& tex)
  {
 
    int edgenum = sqrt(sample_rate);
    float x0_scale = x0 * edgenum, y0_scale = y0 * edgenum,
      x1_scale = x1 * edgenum, y1_scale = y1 * edgenum,
      x2_scale = x2 * edgenum, y2_scale = y2 * edgenum;

    float xmax_scale = max(x0_scale, max(x1_scale, x2_scale));
    float ymax_scale = max(y0_scale, max(y1_scale, y2_scale));
    float xmin_scale = min(x0_scale, min(x1_scale, x2_scale));
    float ymin_scale = min(y0_scale, min(y1_scale, y2_scale));

    size_t width_scale = width * edgenum;
    size_t height_scale = height * edgenum;


    Matrix3x3 m_screen(x0_scale, x1_scale, x2_scale, y0_scale, y1_scale, y2_scale, 1, 1, 1);
    Matrix3x3 m_screen_inv = m_screen.inv();
    Matrix3x3 m_text(u0, u1, u2, v0, v1, v2, 1, 1, 1);
    
    // TODO: Task 5: Fill in the SampleParams struct and pass it to the tex.sample function.
    SampleParams smple;
    smple.lsm = lsm;
    smple.psm = psm;
      
    for (int x = xmin_scale; x < xmax_scale; ++x) {
      int sx = (int)floor(x);
      if (sx < 0 || sx >= width_scale) break;
      for (int y = ymin_scale; y < ymax_scale; ++y) {
        int sy = (int)floor(y);
        if (sy < 0 || sy >= height_scale) break;
        float xtest = x + 0.5;
        float ytest = y + 0.5;
             
        // if the pixel sampling point is in triangle
        if (insidetriangle(x0_scale, y0_scale, x1_scale, y1_scale, x2_scale, y2_scale, xtest, ytest)) {
          // we convert it into uv
          Vector3D xy(x, y, 1); // screenpoint pixel
          Vector3D w_uv = m_screen_inv * xy;
          Vector3D w_dx_uv = m_screen_inv * (xy + Vector3D(1, 0, 0));
          Vector3D w_dy_uv = m_screen_inv * (xy + Vector3D(0, 1, 0));
                   
          Vector3D adj_w(w_uv.x, w_uv.y, 1.0 - w_uv.x - w_uv.y); // make sure all the weights add up to 1
          Vector3D adj_w_dx(w_dx_uv.x, w_dx_uv.y, 1.0 - w_dx_uv.x - w_dx_uv.y); // make sure all the weights add up to 1
          Vector3D adj_w_dy(w_dy_uv.x, w_dy_uv.y, 1.0 - w_dy_uv.x - w_dy_uv.y); // make sure all the weights add up to 1


          //get texture coordinates of uv, (u+1, v), and (u, v+1) 
          Vector3D uv_text = m_text * adj_w; // convert this into uv texture sampling spac
          Vector3D dx_uv_text = m_text * adj_w_dx; // (u+1)
          Vector3D dy_uv_text = m_text * adj_w_dy; // (v+1)

          // TODO: Task 6: Set the correct barycentric differentials in the SampleParams struct.
         // Hint: You can reuse code from rasterize_triangle/rasterize_interpolated_color_triangle

          //now set the remaining parameters in SampleParams smple
          smple.p_uv = Vector2D(uv_text.x, uv_text.y);  
          smple.p_dx_uv = Vector2D(dx_uv_text.x, dx_uv_text.y); 
          smple.p_dy_uv = Vector2D(dy_uv_text.x, dx_uv_text.y); 
          //get color from sample
          Color c = tex.sample(smple);
          sample_buffer[sy * width_scale + sx] = c; 
        }
      }
    }
  }

  void RasterizerImp::set_sample_rate(unsigned int rate) {
    // TODO: Task 2: You may want to update this function for supersampling support

    this->sample_rate = rate;


    this->sample_buffer.resize(width * height * rate, Color::White);
  }


  void RasterizerImp::set_framebuffer_target(unsigned char* rgb_framebuffer,
    size_t width, size_t height)
  {
    // TODO: Task 2: You may want to update this function for supersampling support

    this->width = width;
    this->height = height;
    this->rgb_framebuffer_target = rgb_framebuffer;

    this->sample_buffer.resize(width * height * sample_rate, Color::White);
  }


  void RasterizerImp::clear_buffers() {
    std::fill(rgb_framebuffer_target, rgb_framebuffer_target + 3 * width * height, 255);
    std::fill(sample_buffer.begin(), sample_buffer.end(), Color::White);
  }


  // This function is called at the end of rasterizing all elements of the
  // SVG file.  If you use a supersample buffer to rasterize SVG elements
  // for antialising, you could use this call to fill the target framebuffer
  // pixels from the supersample buffer data.
  //
  void RasterizerImp::resolve_to_framebuffer() {
    // TODO: Task 2: You will likely want to update this function for supersampling support
    int sample_rate = this->sample_rate;
    size_t width_scale = width * sqrt(sample_rate);
    size_t height_scale = height * sqrt(sample_rate);
    for (int x = 0; x < width; ++x) {
      for (int y = 0; y < height; ++y) {
        //now we can downsample from super sampling
        Color col;
        float avg_down = 1.0 / sample_rate;
        int x_sub = x * sqrt(sample_rate);
        int y_sub = y * sqrt(sample_rate);
        for (int xa = x_sub; xa < (x_sub + sqrt(sample_rate)); ++xa) {
            for (int ya = y_sub; ya < (y_sub + sqrt(sample_rate)); ++ya) {
                col += sample_buffer[ya * width_scale + xa] * avg_down;
            }
        }
        for (int k = 0; k < 3; ++k)  {
          this->rgb_framebuffer_target[3 * (y * width + x) + k] = (&col.r)[k] * 255;
        }
      }
    }
  }

  Rasterizer::~Rasterizer() { }


}// CGL
