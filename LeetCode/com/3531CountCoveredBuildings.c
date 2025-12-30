#include <stdlib.h>
#include <string.h>

/**
 * Initialize row and column boundaries for all buildings
 * For each row: track min/max column indices
 * For each column: track min/max row indices
 */
void initializeBoundaries(int gridSize, int **buildings, int buildingsSize,
                         int *rowMaxCols, int *rowMinCols, 
                         int *colMaxRows, int *colMinRows) {
	int i, currentRow, currentCol;

	// Initialize boundaries
	for (i = 0; i <= gridSize; i++) {
		rowMaxCols[i] = 0;
		rowMinCols[i] = gridSize + 1;
		colMaxRows[i] = 0;
		colMinRows[i] = gridSize + 1;
	}

	// Calculate boundaries for each building position
	for (i = 0; i < buildingsSize; i++) {
		currentCol = buildings[i][0];  // x coordinate
		currentRow = buildings[i][1];  // y coordinate

		// Update column boundaries for this row
		if (currentCol > rowMaxCols[currentRow])
			rowMaxCols[currentRow] = currentCol;
		if (currentCol < rowMinCols[currentRow])
			rowMinCols[currentRow] = currentCol;
		
		// Update row boundaries for this column
		if (currentRow > colMaxRows[currentCol])
			colMaxRows[currentCol] = currentRow;
		if (currentRow < colMinRows[currentCol])
			colMinRows[currentCol] = currentRow;
	}
}

/**
 * Count buildings that are completely covered by other buildings
 * A building is covered if it's not on the boundary in both row and column
 */
int countFullyCoveredBuildings(int gridSize, int **buildings, int buildingsSize,
                               int *rowMaxCols, int *rowMinCols,
                               int *colMaxRows, int *colMinRows) {
	int i, currentRow, currentCol, coveredCount = 0;

	for (i = 0; i < buildingsSize; i++) {
		currentCol = buildings[i][0];  // x coordinate
		currentRow = buildings[i][1];  // y coordinate

		// Check if building is not on boundary in both directions
		if (currentCol > rowMinCols[currentRow] && currentCol < rowMaxCols[currentRow] &&
		    currentRow > colMinRows[currentCol] && currentRow < colMaxRows[currentCol]) {
			coveredCount++;
		}
	}

	return coveredCount;
}

/**
 * LeetCode 3531: Count Buildings Surrounded by Other Buildings
 * 
 * Algorithm:
 * 1. For each row, track the leftmost and rightmost building columns
 * 2. For each column, track the topmost and bottommost building rows
 * 3. A building is "covered" if it's not on the boundary in both dimensions
 * 
 * Time Complexity: O(m) where m is the number of buildings
 * Space Complexity: O(n) where n is the grid size
 */
int countCoveredBuildings(int n, int** buildings, int buildingsSize, int* buildingsColSize) {
	int *rowMaxCols, *rowMinCols, *colMaxRows, *colMinRows;
	int coveredCount;

	// Allocate boundary tracking arrays
	rowMaxCols = (int *)malloc((n + 1) * sizeof(int));
	rowMinCols = (int *)malloc((n + 1) * sizeof(int));
	colMaxRows = (int *)malloc((n + 1) * sizeof(int));
	colMinRows = (int *)malloc((n + 1) * sizeof(int));

	// Calculate boundaries
	initializeBoundaries(n, buildings, buildingsSize, 
	                    rowMaxCols, rowMinCols, colMaxRows, colMinRows);

	// Count covered buildings
	coveredCount = countFullyCoveredBuildings(n, buildings, buildingsSize,
	                                         rowMaxCols, rowMinCols, 
	                                         colMaxRows, colMinRows);

	// Free memory
	free(rowMaxCols);
	free(rowMinCols);
	free(colMaxRows);
	free(colMinRows);

	return coveredCount;
}