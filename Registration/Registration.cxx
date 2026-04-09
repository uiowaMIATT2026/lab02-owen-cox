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
template <unsigned int Dimension>
typename itk::EllipseSpatialObject<Dimension>
drawCircle() {
    using EllipseType = itk::EllipseSpatialObject<Dimension>;

    auto circle = EllipseType::New();

    // Set radius, position
    // First circle radius 30.0 mm -> diameter 60.0
    circle->SetRadiusInObjectSpace(60.0);

    // We need perform a translation to set circle position
    using TransformType = typename EllipseType::TransformType;
    auto transform = TransformType::New();
    transform->SetIdentity();

    typename TransformType::OutputVectorType translation;
    translation[0] = 50.0;
    translation[1] = 50.0;

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

    auto circle = drawCircle<2>();


    return EXIT_SUCCESS;

}
