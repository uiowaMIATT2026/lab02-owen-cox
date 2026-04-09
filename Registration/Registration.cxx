//
// Created by ocx on 4/9/26.
//

#include "RegistrationCLP.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"


// Ellipse function sounds useful for circles
#include "itkEllipseSpatialObject.h"
#include "itkSpatialObjectToImageFilter.h"

// Registration includes
#include "itkImageRegistrationMethodv4.h"
#include "itkTranslationTransform.h"
#include "itkMeanSquaresImageToImageMetricv4.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"

// Design for Dimension == 2 for now
// May want to make radius, x, y type float or double
// I will use unsigned int for testing
template <unsigned int Dimension>
typename itk::EllipseSpatialObject<Dimension>::Pointer
drawCircle(double radius, double x, double y) {
    using EllipseType = itk::EllipseSpatialObject<Dimension>;

    auto circle = EllipseType::New();

    // Set radius, position
    // First circle radius 315.0 mm -> diameter 30.0
    circle->SetRadiusInObjectSpace(radius);

    // We need perform a translation to set circle position
    using TransformType = typename EllipseType::TransformType;
    auto transform = TransformType::New();
    transform->SetIdentity();

    typename TransformType::OutputVectorType translation;
    translation[0] = x;
    translation[1] = y;

    transform->Translate(translation, false);
    circle->SetObjectToParentTransform(transform);

    // I probably want to make this an image

    return circle;
}

int main(int argc, char* * argv) {
    PARSE_ARGS;

    constexpr size_t Dimension = 2;

    using EllipseType = itk::EllipseSpatialObject<Dimension>;
    using PixelType = float;

    //auto circle = EllipseType::New();

    auto circle0 = drawCircle<Dimension>(15.0, 50.0, 50.0);
    auto circle1 = drawCircle<Dimension>(30.0, 200.0, 200.0);

    // Probably want to use a similarity transform since we are only doing translation and scaling

    // How do I turn circles into an image?
    using FixedImageType = itk::Image<PixelType, Dimension>;
    using MovingImageType = itk::Image<PixelType, Dimension>;

    using TransformType = itk::TranslationTransform<double, Dimension>;
    using OptimizerType = itk::RegularStepGradientDescentOptimizerv4<double>;
    using MetricType = itk::MeanSquaresImageToImageMetricv4<FixedImageType, MovingImageType>;
    using RegistrationType = itk::ImageRegistrationMethodv4<FixedImageType, MovingImageType, TransformType>;

    auto metric = MetricType::New();
    auto optimizer = OptimizerType::New();
    auto registration = RegistrationType::New();

    registration->SetMetric(metric);
    registration->SetOptimizer(optimizer);

    using FixedLinearInterpolatorType = itk::LinearInterpolateImageFunction<FixedImageType, double>;
    using MovingLinearInterpolatorType = itk::LinearInterpolateImageFunction<MovingImageType, double>;

    auto fixedInterpolator = FixedLinearInterpolatorType::New();
    auto movingInterpolator = MovingLinearInterpolatorType::New();

    metric->SetFixedInterpolator(fixedInterpolator);
    metric->SetMovingInterpolator(movingInterpolator);



    return EXIT_SUCCESS;

}
