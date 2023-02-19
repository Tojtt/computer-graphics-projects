#include "texture.h"
#include "CGL/color.h"
#include "Vector4D.h"

#include <cmath>
#include <algorithm>

namespace CGL {

  Color Texture::sample(const SampleParams& sp) {
    // TODO: Task 6: Fill this in.
    if (sp.p_uv.x > 1 || sp.p_uv.x < 0 || sp.p_uv.y > 1 || sp.p_uv.y < 0) {
      return Color(1, 1, 1);
    }

    float level = get_level(sp);
    switch (sp.lsm) {
    case L_ZERO:
      level = 0;
      if (sp.psm == P_NEAREST) {
        return sample_nearest(sp.p_uv, level);
      } 
      else if (sp.psm == P_LINEAR) {
        return sample_bilinear(sp.p_uv, level);
      }
    break;


    case L_NEAREST:
      level = round(level);
      //make sure we are using valid mip map level
      level = (level < 0) ? 0 : level;
      level = (level > kMaxMipLevels) ? kMaxMipLevels : level;

      if (sp.psm == P_NEAREST) {
        return sample_nearest(sp.p_uv, level);
      }
      else if (sp.psm == P_LINEAR) {
        return sample_bilinear(sp.p_uv, level);
      }
    break;
    case L_LINEAR:
      float top = ceil(level);
      float bottom = floor(level);
      
      //make sure we are blending valid mip map level
      top = (top < 0) ? 0 : top;
      top = (top > kMaxMipLevels) ? kMaxMipLevels : top;

      bottom = (bottom < 0) ? 0 : bottom;
      bottom = (bottom > kMaxMipLevels) ? kMaxMipLevels : bottom;

      if (sp.psm == P_NEAREST) {
        return (sample_nearest(sp.p_uv, top) + sample_nearest(sp.p_uv, bottom)) * 0.5;
      }
      else if (sp.psm == P_LINEAR) {
        return (sample_bilinear(sp.p_uv, top) + sample_bilinear(sp.p_uv, bottom)) * 0.5;
      }
    break;
    }
  }

  float Texture::get_level(const SampleParams& sp) {
    // TODO: Task 6: Fill this in.

    Vector2D dx_uv = sp.p_uv - sp.p_dx_uv;
    Vector2D dy_uv = sp.p_uv - sp.p_dy_uv;

    Vector2D dx_uv_scale(dx_uv.x * (width - 1), dx_uv.y * (height - 1));
    Vector2D dy_uv_scale(dy_uv.x * (width - 1), dy_uv.y * (height - 1));

    float L = max(dx_uv_scale.norm(), dy_uv_scale.norm());
    float D = log2(L); 
    return D;  
  }

  Color MipLevel::get_texel(int tx, int ty) {
    return Color(&texels[tx * 3 + ty * width * 3]);
  }

  Color Texture::sample_nearest(Vector2D uv, int level) {
    // TODO: Task 5: Fill this in.
      // if level is invalid return magenta
    if (level > kMaxMipLevels || level < 0) {
      return  Color(1, 0, 1);
    }
    auto& mip = mipmap[level];
    //get texture coordinate based on uv mapping 
    int texel_x = round(uv.x * (mip.width - 1)), texel_y = round(uv.y * (mip.height - 1));
    //return texel
    Color c = mip.get_texel(texel_x, texel_y);
    return c;
  }


  Color Texture::sample_bilinear(Vector2D uv, int level) {
    // TODO: Task 5: Fill this in.
    auto& mip = mipmap[level];
    if (level > kMaxMipLevels || level < 0) {
      return  Color(1, 0, 1);
    }

    //scale uv into coordinate in the mipmap
    Vector2D uv_scale(uv.x * (mip.width - 1), uv.y * (mip.height - 1));
    //get texture values of sample locations
    //   c2---c3
    //    |   |
    //   c1---c4
    Color c1 = mip.get_texel(floor(uv_scale.x), floor(uv_scale.y));
    Color c2 = mip.get_texel(floor(uv_scale.x), ceil(uv_scale.y));
    Color c3 = mip.get_texel(ceil(uv_scale.x), ceil(uv_scale.y));
    Color c4 = mip.get_texel(ceil(uv_scale.x), floor(uv_scale.y));

    // lerp horizontally
    float s = uv_scale.x - floor(uv_scale.x); //calculate s for lerp
    float t = uv_scale.y - floor(uv_scale.y); //calculate t for lerp
    Color u1 = c1 + s * (c4 + (-1 * c1));
    Color u2 = c2 + s * (c3 + (-1 * c2));

    // lerp vertically
    Color c = u1 + t * (u2 + (-1 * u1));
    return c;
  }





  /****************************************************************************/

  // Helpers

  inline void uint8_to_float(float dst[3], unsigned char* src) {
    uint8_t* src_uint8 = (uint8_t*)src;
    dst[0] = src_uint8[0] / 255.f;
    dst[1] = src_uint8[1] / 255.f;
    dst[2] = src_uint8[2] / 255.f;
  }

  inline void float_to_uint8(unsigned char* dst, float src[3]) {
    uint8_t* dst_uint8 = (uint8_t*)dst;
    dst_uint8[0] = (uint8_t)(255.f * max(0.0f, min(1.0f, src[0])));
    dst_uint8[1] = (uint8_t)(255.f * max(0.0f, min(1.0f, src[1])));
    dst_uint8[2] = (uint8_t)(255.f * max(0.0f, min(1.0f, src[2])));
  }

  void Texture::generate_mips(int startLevel) {

    // make sure there's a valid texture
    if (startLevel >= mipmap.size()) {
      std::cerr << "Invalid start level";
    }

    // allocate sublevels
    int baseWidth = mipmap[startLevel].width;
    int baseHeight = mipmap[startLevel].height;
    int numSubLevels = (int)(log2f((float)max(baseWidth, baseHeight)));

    numSubLevels = min(numSubLevels, kMaxMipLevels - startLevel - 1);
    mipmap.resize(startLevel + numSubLevels + 1);

    int width = baseWidth;
    int height = baseHeight;
    for (int i = 1; i <= numSubLevels; i++) {

      MipLevel& level = mipmap[startLevel + i];

      // handle odd size texture by rounding down
      width = max(1, width / 2);
      //assert (width > 0);
      height = max(1, height / 2);
      //assert (height > 0);

      level.width = width;
      level.height = height;
      level.texels = vector<unsigned char>(3 * width * height);
    }

    // create mips
    int subLevels = numSubLevels - (startLevel + 1);
    for (int mipLevel = startLevel + 1; mipLevel < startLevel + subLevels + 1;
      mipLevel++) {

      MipLevel& prevLevel = mipmap[mipLevel - 1];
      MipLevel& currLevel = mipmap[mipLevel];

      int prevLevelPitch = prevLevel.width * 3; // 32 bit RGB
      int currLevelPitch = currLevel.width * 3; // 32 bit RGB

      unsigned char* prevLevelMem;
      unsigned char* currLevelMem;

      currLevelMem = (unsigned char*)&currLevel.texels[0];
      prevLevelMem = (unsigned char*)&prevLevel.texels[0];

      float wDecimal, wNorm, wWeight[3];
      int wSupport;
      float hDecimal, hNorm, hWeight[3];
      int hSupport;

      float result[3];
      float input[3];

      // conditional differentiates no rounding case from round down case
      if (prevLevel.width & 1) {
        wSupport = 3;
        wDecimal = 1.0f / (float)currLevel.width;
      }
      else {
        wSupport = 2;
        wDecimal = 0.0f;
      }

      // conditional differentiates no rounding case from round down case
      if (prevLevel.height & 1) {
        hSupport = 3;
        hDecimal = 1.0f / (float)currLevel.height;
      }
      else {
        hSupport = 2;
        hDecimal = 0.0f;
      }

      wNorm = 1.0f / (2.0f + wDecimal);
      hNorm = 1.0f / (2.0f + hDecimal);

      // case 1: reduction only in horizontal size (vertical size is 1)
      if (currLevel.height == prevLevel.height) {
        //assert (currLevel.height == 1);

        for (int i = 0; i < currLevel.width; i++) {
          wWeight[0] = wNorm * (1.0f - wDecimal * i);
          wWeight[1] = wNorm * 1.0f;
          wWeight[2] = wNorm * wDecimal * (i + 1);

          result[0] = result[1] = result[2] = 0.0f;

          for (int ii = 0; ii < wSupport; ii++) {
            uint8_to_float(input, prevLevelMem + 3 * (2 * i + ii));
            result[0] += wWeight[ii] * input[0];
            result[1] += wWeight[ii] * input[1];
            result[2] += wWeight[ii] * input[2];
          }

          // convert back to format of the texture
          float_to_uint8(currLevelMem + (3 * i), result);
        }

        // case 2: reduction only in vertical size (horizontal size is 1)
      }
      else if (currLevel.width == prevLevel.width) {
        //assert (currLevel.width == 1);

        for (int j = 0; j < currLevel.height; j++) {
          hWeight[0] = hNorm * (1.0f - hDecimal * j);
          hWeight[1] = hNorm;
          hWeight[2] = hNorm * hDecimal * (j + 1);

          result[0] = result[1] = result[2] = 0.0f;
          for (int jj = 0; jj < hSupport; jj++) {
            uint8_to_float(input, prevLevelMem + prevLevelPitch * (2 * j + jj));
            result[0] += hWeight[jj] * input[0];
            result[1] += hWeight[jj] * input[1];
            result[2] += hWeight[jj] * input[2];
          }

          // convert back to format of the texture
          float_to_uint8(currLevelMem + (currLevelPitch * j), result);
        }

        // case 3: reduction in both horizontal and vertical size
      }
      else {

        for (int j = 0; j < currLevel.height; j++) {
          hWeight[0] = hNorm * (1.0f - hDecimal * j);
          hWeight[1] = hNorm;
          hWeight[2] = hNorm * hDecimal * (j + 1);

          for (int i = 0; i < currLevel.width; i++) {
            wWeight[0] = wNorm * (1.0f - wDecimal * i);
            wWeight[1] = wNorm * 1.0f;
            wWeight[2] = wNorm * wDecimal * (i + 1);

            result[0] = result[1] = result[2] = 0.0f;

            // convolve source image with a trapezoidal filter.
            // in the case of no rounding this is just a box filter of width 2.
            // in the general case, the support region is 3x3.
            for (int jj = 0; jj < hSupport; jj++)
              for (int ii = 0; ii < wSupport; ii++) {
                float weight = hWeight[jj] * wWeight[ii];
                uint8_to_float(input, prevLevelMem +
                  prevLevelPitch * (2 * j + jj) +
                  3 * (2 * i + ii));
                result[0] += weight * input[0];
                result[1] += weight * input[1];
                result[2] += weight * input[2];
              }

            // convert back to format of the texture
            float_to_uint8(currLevelMem + currLevelPitch * j + 3 * i, result);
          }
        }
      }
    }
  }

}
