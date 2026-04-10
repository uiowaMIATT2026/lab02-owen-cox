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
#include "itkSimilarity2DTransform.h"
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

    return circle;
}

// Do not try to run main, this seems to break things somehow
int main(int argc, char* * argv) {
    PARSE_ARGS;

    constexpr size_t Dimension = 2;

    using CircleType = itk::EllipseSpatialObject<Dimension>;
    using PixelType = float;
    using ImageType = itk::Image<PixelType, Dimension>;

    //auto circle = EllipseType::New();

    auto circle0 = drawCircle<Dimension>(15.0, 50.0, 50.0);
    auto circle1 = drawCircle<Dimension>(30.0, 200.0, 200.0);

    using FixedImageType = itk::Image<PixelType, Dimension>;
    using MovingImageType = itk::Image<PixelType, Dimension>;

    using RasterizeFilterType = itk::SpatialObjectToImageFilter<CircleType, ImageType>;

    auto rasterize = RasterizeFilterType::New();
    rasterize->SetInput(circle0);

    ImageType::SizeType size;
    size[0] = 512;
    size[1] = 512;
    rasterize->SetSize(size);

    ImageType::SpacingType spacing;
    spacing[0] = 1.0;
    spacing[1] = 1.0;
    rasterize->SetSpacing(spacing);

    ImageType::PointType origin;
    origin[0] = 0.0;
    origin[1] = 0.0;
    rasterize->SetOrigin(origin);

    rasterize->SetInsideValue(255);
    rasterize->SetOutsideValue(0);

    // May want try-catch blocks here
    rasterize->Update();

    FixedImageType::Pointer circleImage0 = rasterize->GetOutput();

    rasterize->SetInput(circle1);
    rasterize->Update();

    MovingImageType::Pointer circleImage1 = rasterize->GetOutput();





    // Probably want to use a similarity transform since we are only doing translation and scaling
    // This is all from the ITK Software Guide Registration Hello World

    // using TransformType = itk::TranslationTransform<double, Dimension>;
    using TransformType = itk::Similarity2DTransform<double>;
    using OptimizerType = itk::RegularStepGradientDescentOptimizerv4<double>;
    using MetricType = itk::MeanSquaresImageToImageMetricv4<FixedImageType, MovingImageType>;
    using RegistrationType = itk::ImageRegistrationMethodv4<FixedImageType, MovingImageType, TransformType>;

    auto metric = MetricType::New();
    auto optimizer = OptimizerType::New();
    auto registration = RegistrationType::New();

    registration->SetMetric(metric);
    registration->SetOptimizer(optimizer);

    registration->SetFixedImage(circleImage0);
    registration->SetMovingImage(circleImage1);

    auto movingInitialTransform = TransformType::New();
    movingInitialTransform->SetIdentity();

    TransformType::InputPointType center;
    center[0] = 200.0;
    center[1] = 200.0;
    movingInitialTransform->SetCenter(center);

    TransformType::ParametersType initialParameters(movingInitialTransform->GetNumberOfParameters());

    initialParameters[0] = 1.0;
    initialParameters[1] = 0.0;
    initialParameters[2] = 0.0;
    initialParameters[3] = 0.0;

    movingInitialTransform->SetParameters(initialParameters);
    registration->SetMovingInitialTransform(movingInitialTransform);

    auto fixedInitialTransform = TransformType::New();
    fixedInitialTransform->SetIdentity();
    registration->SetFixedInitialTransform(fixedInitialTransform);

    OptimizerType::ScalesType optimizerScales(movingInitialTransform->GetNumberOfParameters());

    optimizerScales[0] = 1.0;
    optimizerScales[1] = 1.0;
    optimizerScales[2] = 0.001;
    optimizerScales[3] = 0.001;

    optimizer->SetLearningRate(1.0);
    optimizer->SetMinimumStepLength(0.0001);
    optimizer->SetRelaxationFactor(0.5);
    optimizer->SetNumberOfIterations(200);

    constexpr unsigned int numberOfLevels = 1;

    RegistrationType::ShrinkFactorsArrayType shrinkFactorsPerLevel;
    shrinkFactorsPerLevel.SetSize(1);
    shrinkFactorsPerLevel[0] = 1;

    RegistrationType::SmoothingSigmasArrayType smoothingSigmasPerLevel;
    smoothingSigmasPerLevel.SetSize(1);
    smoothingSigmasPerLevel[0] = 0;

    registration->SetNumberOfLevels(numberOfLevels);
    registration->SetSmoothingSigmasPerLevel(smoothingSigmasPerLevel);
    registration->SetShrinkFactorsPerLevel(shrinkFactorsPerLevel);

    try {
        registration->Update();

        const auto finalParameters = registration->GetTransform()->GetParameters();

        std::cout << "Final scale: " << finalParameters[0] << std::endl;
        std::cout << "Final angle: " << finalParameters[1] << std::endl;
        std::cout << "Final tx: " << finalParameters[2] << std::endl;
        std::cout << "Final ty: " << finalParameters[3] << std::endl;


    } catch (const itk::ExceptionObject & error) {
        std::cerr << "Error: " << error << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;

}
