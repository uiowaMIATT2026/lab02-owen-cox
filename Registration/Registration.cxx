//
// Created by ocx on 4/9/26.
//

#include "RegistrationCLP.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"


// Ellipse function sounds useful
#include "itkEllipseSpatialObject.h"


int main(int argc, char* * argv) {
    PARSE_ARGS;

    constexpr size_t Dimension = 2;

    using EllipseType = itk::EllipseSpatialObject<Dimension>;
    using PixelType = float;

    auto circle = EllipseType::New();


    return EXIT_SUCCESS;

}
