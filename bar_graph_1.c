#include "pbPlots.h"
#include "supportLib.h"

// Declaration of DrawBarPlot function
RGBABitmapImage *DrawBarPlot(double width, double height, double *ys, size_t ysLength);

int main()
{
    double ys[] = {10, 20, 30, 40, 50};           // Sample data for y-values
    size_t ysLength = sizeof(ys) / sizeof(ys[0]); // Length of the y-values array

    double width = 800;  // Width of the canvas
    double height = 600; // Height of the canvas
    size_t length;

    // Draw the bar plot
    RGBABitmapImage *barPlotImage = DrawBarPlot(width, height, ys, ysLength);
    double *pngData = ConvertToPNG(&length, barPlotImage);
    // Save the bar plot image to a PNG file
    WriteToFile(pngData, length, "bar_graph_1.png");

    // Free the memory allocated for the bar plot image
    // FreeImage(barPlotImage);

    return 0;
}
