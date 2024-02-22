#include "pch.h"
#include "Renderer.h"
#include "tinyexr.h"
namespace Alalba
{
  Renderer::Renderer(signed int width, signed int height, double deltaS)
    :m_width(width),m_height(height), m_rayMarcherDeltaS(deltaS)
  {

  }

  void Renderer::Render(const lux::Camera& camera, const ScalarField& densityField, const ColorField& colorField)
  {
    m_rayMarcher.reset(new RayMarcher(camera, m_width, m_height, m_rayMarcherDeltaS));
    m_rayMarcher->RayMarch(densityField, colorField);
    
    m_frameBuffer = m_rayMarcher->GetResult();
  }

  bool Renderer::SaveImage( const char* outfilename)
  {
    float* channels[4];

    EXRImage image;
    InitEXRImage(&image);

    image.num_channels = 4;

    // Must be BGR(A) order, since most of EXR viewers expect this channel order.
    const char* channel_names[] = {"A", "B", "G", "R"}; // "B", "G", "R", "A" for RGBA image

    std::vector<float> images[4];
    images[0].resize(m_width * m_height);
    images[1].resize(m_width * m_height);
    images[2].resize(m_width * m_height);
    images[3].resize(m_width * m_height);

    for (int row = 0; row < m_height; row++)
      for (int col = 0; col < m_width; col++)
      {
        images[0][(m_height-row-1) * m_width + col] = m_frameBuffer[row * m_width + col](0);
        images[1][(m_height-row-1) * m_width + col] = m_frameBuffer[row * m_width + col](1);
        images[2][(m_height-row-1) * m_width + col] = m_frameBuffer[row * m_width + col](2);
        images[3][(m_height-row-1) * m_width + col] = m_frameBuffer[row * m_width + col](3);
      }
    //for (int i = 0; i < m_width * m_height; i++) {
    //  
    //  images[0][i] = m_frameBuffer[i](0);
    //  images[1][i] = m_frameBuffer[i](1);
    //  images[2][i] = m_frameBuffer[i](2);
    //  images[3][i] = m_frameBuffer[i](3);
    //}

    float* image_ptr[4];
    
    image_ptr[0] = &(images[3].at(0)); // A
    image_ptr[1] = &(images[2].at(0)); // B
    image_ptr[2] = &(images[1].at(0)); // G
    image_ptr[3] = &(images[0].at(0)); // R
    

    image.channel_names = channel_names;
    image.images = (unsigned char**)image_ptr;
    image.width = m_width;
    image.height = m_height;
    image.line_order = 1;
    image.compression = TINYEXR_COMPRESSIONTYPE_ZIP;

    image.pixel_types = (int*)malloc(sizeof(int) * image.num_channels);
    image.requested_pixel_types = (int*)malloc(sizeof(int) * image.num_channels);
    for (int i = 0; i < image.num_channels; i++) {
      image.pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT; // pixel type of input image
      image.requested_pixel_types[i] = TINYEXR_PIXELTYPE_HALF; // pixel type of output image to be stored in .EXR
    }

    const char* err;
    int ret = SaveMultiChannelEXRToFile(&image, outfilename, &err);
    if (ret != 0) {
      fprintf(stderr, "Save EXR err: %s\n", err);
      return ret;
    }
    ALALBA_TRACE("Saved exr file. {0}", outfilename);

    free(image.pixel_types);
    free(image.requested_pixel_types);
    return ret;

  }

}