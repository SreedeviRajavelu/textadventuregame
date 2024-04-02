// #include "plot_graph.h"

// void plot_graph(Player *player, const char *filename, const char *csvfilename)
// {
//     printf("Enter name for graph file to be save and then press enter:\n");
//     char graphName[50];
//     fgets(graphName, sizeof(graphName), stdin);
//     graphName[strcspn(graphName, "\n")] = '\0';

//     // prepare data for the bar graph
//     size_t numItems = 0;
//     double quantities[10];
//     const char *itemNames[10];

//     // extract item names and quantities from the player's inventory
//     for (int i = 0; i < 10; i++)
//     {
//         if (player->items[i].quantity > 0)
//         {
//             itemNames[numItems] = player->items[i].itemName;
//             quantities[numItems] = player->items[i].quantity;
//             numItems++;
//         }

//         // create the bar plot

//         double width = 800;
//         double height = 600;
//         RGBABitmapImage *barPlotImage = DrawBarPlot(width, height, quantities, numItems);

//         // Add title to the bar graph using the custom function
//         // DrawTitle(barPlotImage, "Inventory");
//         // Reference to code from pbPlots.c
//         // DrawText(canvas, floor(ImageWidth(canvas)/2.0 - GetTextWidth(settings->title, settings->titleLength)/2.0), floor(yPadding/3.0), settings->title, settings->titleLength, GetBlack());
//         // DrawTextUpwards(canvas, 10.0, floor(ImageHeight(canvas)/2.0 - GetTextWidth(settings->yLabel, settings->yLabelLength)/2.0), settings->yLabel, settings->yLabelLength, GetBlack());
//         // Add labels to the x-axis (item names)
//         // for (size_t i = 0; i < numItems; i++)
//         // {
//         //     DrawText(barPlotImage, 70 + i * 70, 550, itemNames[i], 12, CreateRGBColor(0, 0, 0), width / 100);
//         // }

//         // // Add label to the y-axis
//         // DrawText(barPlotImage, 20, 300, "Quantity", 12, CreateRGBColor(0, 0, 0), width / 100);

//         size_t length;
//         double *pngData = ConvertToPNG(&length, barPlotImage);

//         // Save the bar plot image to a file
//         WriteToFile(pngData, length, graphName);
//     }
// }