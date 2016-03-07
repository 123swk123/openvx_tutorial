/*
 * Copyright (c) 2016 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and/or associated documentation files (the
 * "Materials"), to deal in the Materials without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Materials, and to
 * permit persons to whom the Materials are furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Materials.
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
 */

/*!
 * \file main.cpp
 * \example easy_exercise1
 * \brief Harris corners example. 
 * Look for TODO keyword in comments to code snipets that you need write.
 * \author Radhakrishna Giduthuri <radha.giduthuri@ieee.org>
 */

////////
// include OpenCV wrapper for image capture and display
#include "opencv_camera_display.h"

////////********
// Most important top-level OpenVX header files are "VX/vx.h" and "VX/vxu.h".
// The "VX/vx.h" includes all headers needed to support functionality of the
// OpenVX specification, except for immediate mode functions, and it includes:
//    VX/vx_types.h     -- type definitions required by the OpenVX standard
//    VX/vx_api.h       -- All framework API definitions
//    VX/vx_kernels.h   -- list of supported kernels in the OpenVX standard 
//    VX/vx_nodes.h     -- 
//    VX/vx_vendors.h
// The "VX/vxu.h" defines immediate mode utility functions.
// TODO:********
//   1. Uncomment the below lines to include OpenVX header files
#include <VX/vx.h>
//#include <VX/vxu.h>

////////
// Useful macros for OpenVX error checking:
//   ERROR_CHECK_STATUS - check status is VX_SUCCESS
//   ERROR_CHECK_OBJECT - check if the object creation is successful
#define ERROR_CHECK_STATUS(status) { \
        vx_status status_ = (status); \
        if(status_ != VX_SUCCESS) { \
            printf("ERROR: failed with status = (%d) at " __FILE__ "#%d\n", status_, __LINE__); \
            exit(1); \
        } \
    }
#define ERROR_CHECK_OBJECT(obj) { \
        vx_status status_ = vxGetStatus((vx_reference)(obj)); \
        if(status_ != VX_SUCCESS) { \
            printf("ERROR: failed with status = (%d) at " __FILE__ "#%d\n", status_, __LINE__); \
            exit(1); \
        } \
    }

////////********
// log_callback function should implements a mechanism to print log messages 
// from OpenVX framework onto console.
// TODO:********
//   1. Complete the log_callback function declaration to match with
//      vx_log_callback_f in VX/vx_types.h (line 1444)
//   2. Make the log_callback print a message to look like:
//        LOG: [<status>] <message>
void log_callback( /* add function arguments here */ )
{
    // ...
}

////////
// main() has all the OpenVX application code for this exercise.
// Command-line usage:
//   % solution_exercise1 [<video-sequence>|<camera-device-number>]
// When neither video sequence nor camera device number is specified, 
// it defaults to the video sequence in "PETS09-S1-L1-View001.avi".
int main( int argc, char * argv[] )
{
    // Get default video sequence when nothing is specified on command-line and
    // instantiate OpenCV GUI module for reading input RGB images and displaying
    // the image with OpenVX results
    const char * video_sequence = (argc > 1) ? argv[1] :
        "/home/openvx/tutorial_videos/PETS09-S1-L1-View001.avi";
    CGuiModule gui( video_sequence );

    // Try grab first video frame from the sequence using cv::VideoCapture
    // and check if video frame is available
    if( !gui.Grab())
    {
        printf( "ERROR: input has no video\n" );
        return 1;
    }

    ////////
    // Set the application configuration parameters. Note that input video
    // sequence is an 8-bit RGB image with dimensions given by gui.GetWidth()
    // and gui.GetHeight(). Harris corners algorithm specific parameters are:
    //   harris_strength_thresh - minimum threshold which to eliminate 
    //                            Harris Corner scores (computed using the
    //                            normalized Sobel kernel)
    //   harris_min_distance    - radial L2 distance for non-max suppression
    //   harris_k_sensitivity   - sensitivity threshold k from the 
    //                            Harris-Stephens 
    //   harris_gradient_size   - gradient window size to use on the input
    //   harris_block_size      - block window size used to compute the 
    //                            harris corner score
    vx_uint32  width                  = gui.GetWidth();
    vx_uint32  height                 = gui.GetHeight();
    vx_float32 harris_strength_thresh = 0.0005f;
    vx_float32 harris_min_distance    = 5.0f;
    vx_float32 harris_k_sensitivity   = 0.04f;
    vx_int32   harris_gradient_size   = 3;
    vx_int32   harris_block_size      = 3;

    ////////********
    // Create the OpenVX context and make sure returned context is valid.
    // TODO:********
    //   1. Create an OpenVX context using the vxCreateContext API.
    //   2. Use vxGetStatus API or ERROR_CHECK_OBJECT macro to check if
    //      context creation is successful.
    //      The vxCreateContext and vxGetStatus are defined in "VX/vx_api.h".
    vx_context context = NULL;
    ERROR_CHECK_OBJECT( context );

    ////////********
    // Register the log_callback that you implemented to be able to receive
    // any log messages from the OpenVX framework.
    // TODO:********
    //   1. Use vxRegisterLogCallback to register the callback. Make sure to
    //      specify reentrant flag as vx_false_e unless you've a reentrant
    //      implementation of the log_callback


    ////////********
    // Create OpenVX image object for input and OpenVX array object for output.
    // TODO:********
    //   1. Use vxCreateImage API for creating image object with the width and
    //      height parameters from configuration parameters. Note that
    //      VX_DF_IMAGE_RGB for RGB image format is defined in "VX/vx_types.h".
    //   2. Use vxCreateArray API for creation of an array object with keypoint
    //      data type. The VX_TYPE_KEYPOINT (for vx_keypoint_t) is defined in
    //      "VX/vx_types.h". Make sure array can hold upto 10,000 keypoints.
    //   3. Use ERROR_CHECK_OBJECT to check proper creation of objects.
    vx_image input_rgb_image = NULL;
    vx_array output_keypoint_array = NULL;
    ERROR_CHECK_OBJECT( input_rgb_image );
    ERROR_CHECK_OBJECT( output_keypoint_array );

    ////////********
    // Harris corner detector algorithm expects input to be an 8-bit image.
    // Given that input is an RGB image, it is best to extract a gray scale
    // from RGB image, which requires two steps:
    //   - perform RGB to IYUV color conversion
    //   - extract Y channel from IYUV image
    // This requires two intermediate OpenVX image objects. Since you're
    // going to use immediate mode functions, you need to use vxCreateImage
    // to create the image objects, but not the vxCreateVirtualImage API.
    // TODO:********
    //   1. Create a IYUV image and a U8 image (for Y channel) with same
    //      dimensions as input RGB image. Note that image formats for
    //      IYUV and U8 images are VX_DF_IMAGE_IYUV and VX_DF_IMAGE_U8.
    //   2. Use ERROR_CHECK_OBJECT to check proper creatio of objects.
    vx_image yuv_image = NULL;
    vx_image gray_scale_image = NULL;
    ERROR_CHECK_OBJECT( yuv_image );
    ERROR_CHECK_OBJECT( gray_scale_image );

    ////////********
    // The immediate mode Harris corner detector function takes the
    // strength_thresh, min_distance, and sensitivity parameters as scalar
    // data objects. So, you need to create three scalar objects with
    // corresponding configuration parameters.
    // TODO:********
    //   1. Create scalar data objects of VX_TYPE_FLOAT32 for strength_thresh,
    //      min_distance, and sensitivity. And make sure to set their initial
    //      values as harris_strength_thresh, harris_min_distance, and
    //      harris_k_sensitivity, respectively.
    //   2. Use ERROR_CHECK_OBJECT to check proper creation of objects.
    vx_scalar strength_thresh = NULL;
    vx_scalar min_distance = NULL;
    vx_scalar sensitivity = NULL;
    ERROR_CHECK_OBJECT( strength_thresh );
    ERROR_CHECK_OBJECT( min_distance );
    ERROR_CHECK_OBJECT( sensitivity );

    ////////
    // process video sequence frame by frame until end of sequence or aborted.
    for( int frame_index = 0; !gui.AbortRequested(); frame_index++ )
    {
        ////////
        // local variable for checking status of OpenVX API calls
        vx_status status;

        ////////********
        // Copy input RGB frame from OpenCV to OpenVX. In order to do this,
        // you need to use vxAccessImagePatch and vxCommitImagePatch APIs.
        // See "VX/vx_api.h" for the description of these APIs.
        // TODO:********
        //   1. Specify the coordinates of image patch by declaring the patch
        //      as a vx_rectangle_t data type. The start_x and start_y should
        //      zeros, whereas, the end_x and end_y should be width and height.
        //   2. Specify the memory layout of OpenCV RGB image buffer by
        //      declaring the layout as a vx_imagepatch_addressing_t type.
        //      Remember that you need to specify stride_x and stride_y fields 
        //      of vx_imagepatch_addressing_t for the image buffer layout. The
        //      stride_x should be 3 and stride_y should be gui.GetStride().
        //   3. Get the pointer to buffer using gui.GetBuffer() and call
        //      vxAccessImagePatch for VX_WRITE_ONLY usage mode with a pointer
        //      to pointer returned by gui.GetBuffer() so COPY mode is used.
        //      Then immediately call vxCommitImagePatch for the actual copy.
        //      Make sure to use the image patch and memory layout in the
        //      above two steps.
        //   4. Compare the return status with VX_SUCCESS to check if access/
        //      commit are successful. Or use ERROR_CHECK_STATUS macro.
        vx_rectangle_t cv_rgb_image_region;
        cv_rgb_image_region.start_x = 0;
        cv_rgb_image_region.start_y = 0;
        cv_rgb_image_region.end_x = width;
        cv_rgb_image_region.end_y = height;
        vx_imagepatch_addressing_t cv_rgb_image_layout;
        cv_rgb_image_layout.stride_x = 3;
        cv_rgb_image_layout.stride_y = gui.GetStride();
        vx_uint8 * cv_rgb_image_buffer = gui.GetBuffer();
        // ...

        ////////********
        // In order to compute Harris corners from input RGB image, first you
        // need to conver input RGB image into a gray scale image, followed by
        // running the Harris corner detector function. All the immediate mode
        // functions you need are declared in "VX/vxu.h".
        // TODO:********
        //   1. Convert input RGB image to IYUV image using vxuColorConvert API.
        //   2. Extract Y channel from IYUV image into gray scale image using
        //      vxuChannelExtract API with VX_CHANNEL_Y as the channel.
        //   3. Compute Harris corner detector using vxuHarrisCorners API.
        //      The num_corners parameter to vxuHarrisCorners is optional
        //      you need to set it to NULL in this exercise.
        //   4. Use ERROR_CHECK_STATUS for error checking.


        ////////********
        // To mark the keypoints in display, you need to access the output
        // keypoint array and draw each item on the output window using
        // gui.DrawPoint().
        // TODO:********
        //   1. OpenVX array object has an attribute that keeps the current
        //      number of items in the array. The name of the attribute is
        //      VX_ARRAY_ATTRIBUTE_NUMITEMS and its value is of type vx_size.
        //      Use vxQueryArray API to get number of keypoints in the
        //      output_keypoint_array data object, representing number of
        //      corners detected in the input RGB image.
        //      IMPORTANT: make sure to read number of items into "num_corners"
        //      because this variable is displayed by code segment below.
        //   2. The data items in output keypoint array are of type
        //      vx_keypoint_t (see "VX/vx_types.h"). To access the array
        //      buffer, use vxAccessArrayRange with start index as ZERO,
        //      end index as number of items in the array, and usage mode as
        //      VX_READ_ONLY. Note that the stride returned by this access
        //      call is not guaranteed to be sizeof(vx_keypoint_t). 
        //      Also make sure that num_corners is > 0, because
        //      vxAccessArrayRange expects end index > 0.
        //   3. For each item in the keypoint buffer, use vxArrayItem to
        //      access individual keypoint and draw a marker at (x,y)
        //      using gui.DrawPoint(). The vx_keypoint_t has x & y data fields.
        //   4. Handover the control of output keypoint buffer back to
        //      OpenVX framework by calling vxCommitArrayRange API.
        //   5. Use ERROR_CHECK_STATUS for error checking.
        vx_size num_corners = 0;
        status = vxQueryArray( output_keypoint_array,
            VX_ARRAY_ATTRIBUTE_NUMITEMS, &num_corners, sizeof( num_corners ));
        ERROR_CHECK_STATUS( status );
        if( num_corners > 0 ) {
            // ...
        }

        ////////
        // Display the results and grab next input RGB frame for next iteration
        char text[128];
        sprintf( text, "Keyboard ESC/Q-Quit SPACE-Pause [FRAME %d]", frame_index );
        gui.DrawText( 0, 16, text );
        sprintf( text, "Number of Corners: %d", (int)num_corners );
        gui.DrawText( 0, 36, text );
        gui.Show();
        if( !gui.Grab())
        {
            // terminate the processing loop if end of sequence is detected
            gui.WaitForKey();
            break;
        }
    }

    ////////********
    // Release all the OpenVX objects created in this exercise and make sure
    // to release the context at the end. To release an OpenVX object, you
    // need to call vxRelease<Object> API which takes a pointer to the object.
    // If the release operation is successful, the OpenVX framework will
    // reset the object to NULL.
    // TODO:********
    //   1. Release all the image objects using vxReleaseImage API.
    //   2. For releasing all other objects use vxRelease<Object> APIs.
    //   3. Use ERROR_CHECK_STATUS for error checking.


    return 0;
}
