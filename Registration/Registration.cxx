//
// Created by ocx on 4/9/26.
//

#include "EdgeSlicerModuleCLP.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include <itkSobelEdgeDetectionImageFilter.h>

int main(int argc, char* * argv) {
    PARSE_ARGS;

    constexpr size_t Dimension = 3;

}
