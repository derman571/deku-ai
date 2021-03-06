#include "eye.hpp"
#define feature_detector_name "ORB"
using std::cout;
typedef unsigned int uint;
/* Possible configurations
Ptr<FeatureDetector> surf = FeatureDetector::create("SURF");
 Ptr<FeatureDetector> star = FeatureDetector::create("STAR");
 Ptr<FeatureDetector> fast = FeatureDetector::create("FAST");
 Ptr<FeatureDetector> sift = FeatureDetector::create("SIFT");
 Ptr<FeatureDetector> orb = FeatureDetector::create("ORB");
 Ptr<FeatureDetector> brisk = FeatureDetector::create("BRISK");
 Ptr<FeatureDetector> mser = FeatureDetector::create("MSER");
 Ptr<FeatureDetector> gftt = FeatureDetector::create("GFTT");
 Ptr<FeatureDetector> harris = FeatureDetector::create("HARRIS");
 Ptr<FeatureDetector> dense = FeatureDetector::create("Dense");
 Ptr<FeatureDetector> simple = FeatureDetector::create("SimpleBlob");*/
//ASSUMES default dislay
Eye::Eye(){
    window = Emulator_Window::get_emulator();
}

cv::Mat Eye::analyze_screen(){
    const unsigned int width = window->width, height = window->height;

    cairo_surface_t* x11_surf = cairo_xlib_surface_create(window->disp, window->root,
                                                          DefaultVisual(window->disp, window->scr),
                                                          width, height);
    cairo_surface_t* img_surf = convert_xlib_to_image_surface(x11_surf, width, height);
    Mat ret;
    convert_image_surface_to_mat(img_surf, width, height, ret);

    //Depreciated
    /*
    vector<KeyPoint> points = analyze_keypoints(ret);
    if(draw_keypoints){
        cv::Mat img_1 = ret;
        cv::Mat img_keypoints_1;
        drawKeypoints( img_1, points, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
        imshow("Keypoints 1", img_keypoints_1 );
        waitKey(0);
    }
    */
    cairo_surface_destroy(x11_surf);
    cairo_surface_destroy(img_surf);
    x11_surf = NULL;
    img_surf = NULL;

    return ret;
}

cairo_surface_t* Eye::convert_xlib_to_image_surface( cairo_surface_t* x11_surf,
  const unsigned int& width, const unsigned int& height){
    cairo_surface_t* img_surf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t* cr = cairo_create(img_surf);
    cairo_set_source_surface(cr, x11_surf, 0, 0);
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint(cr);
    cairo_destroy(cr);

    return img_surf;

}

void Eye::convert_image_surface_to_mat(cairo_surface_t* img_surf, const unsigned int& width, const unsigned int& height, Mat& ret){
    unsigned char* ptr = cairo_image_surface_get_data(img_surf);

    if(!ptr){
        cout<<"ERROR ptr is bad";
    };
    //cairo_surface_write_to_png(img_surf,"test.png");

    Mat img_1 = cv::Mat::zeros(height, width, CV_8U);

    for (int i = 0; i < img_1.rows; ++i)
    {
        for (int j = 0; j < img_1.cols; j++)
        {
            uint alpha = (uint)*ptr;
            ++ptr;
            uint red = (uint)*ptr;
            ++ptr;
            uint blue = (uint)*ptr;
            ++ptr;
            uint green = (uint)*ptr;
            ++ptr;
            unsigned char average = ((unsigned char)((red+blue+green)/3*alpha/255.0));
            img_1.at<unsigned char>(i, j)=average;

        }
    }

    ret = img_1;
}
