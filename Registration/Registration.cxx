//
// Created by ocx on 4/9/26.
//

#include "RegistrationCLP.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"


// Ellipse function sounds useful for circles
#include "itkEllipseSpatialObject.h"


// Design for Dimension == 2 for now
// May want to make radius, x, y type float or double
// I will use unsigned int for testing
template <unsigned int Dimension>
typename itk::EllipseSpatialObject<Dimension>::Pointer
drawCircle(unsigned int radius, unsigned int x, unsigned int y) {
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

int main(int argc, char* * argv) {
    PARSE_ARGS;

    constexpr size_t Dimension = 2;

    using EllipseType = itk::EllipseSpatialObject<Dimension>;
    using PixelType = float;

    //auto circle = EllipseType::New();

    auto circle0 = drawCircle<Dimension>(15, 50, 50);
    auto circle1 = drawCircle<Dimension>(30, 200, 200);


    return EXIT_SUCCESS;

}
