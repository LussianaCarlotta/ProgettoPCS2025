#pragma once

#include <iostream>
#include <vector>
#include <Eigen/Dense>

#include <gtest/gtest.h>
#include "PoliedriMesh.hpp"

namespace PoliedriLibrary {
	
// Test sui vertici

unsigned int NumCell0Ds = 0;
std::vector<unsigned int> Cell0DsId;
Eigen::MatrixXd Cell0DsCoordinates;

void inizializzaVertici(unsigned int num) {
    NumCell0Ds = num;
    Cell0DsId.resize(num);
    Cell0DsCoordinates = Eigen::MatrixXd(3, num);

    for (unsigned int i = 0; i < num; ++i) {
        Cell0DsId[i] = i;
        Cell0DsCoordinates(0, i) = i * 1.0; // componente x
        Cell0DsCoordinates(1, i) = i * 2.0; // componente y
        Cell0DsCoordinates(2, i) = i * 3.0; // componente z
    }
}

TEST(Cell0DsTest, Inizializzazione) {
    inizializzaVertici(3);

    EXPECT_EQ(NumCell0Ds, 3);
    EXPECT_EQ(Cell0DsId.size(), 3);
    EXPECT_EQ(Cell0DsCoordinates.cols(), 3);
    EXPECT_EQ(Cell0DsCoordinates.rows(), 3);

    for (unsigned int i = 0; i < 3; ++i) {
        EXPECT_EQ(Cell0DsId[i], i);
        EXPECT_DOUBLE_EQ(Cell0DsCoordinates(0, i), i * 1.0);
        EXPECT_DOUBLE_EQ(Cell0DsCoordinates(1, i), i * 2.0);
        EXPECT_DOUBLE_EQ(Cell0DsCoordinates(2, i), i * 3.0);
    }
}


// Test sui lati

unsigned int NumCell1Ds = 0;
std::vector<unsigned int> Cell1DsId;
Eigen::MatrixXi Cell1DsExtrema;

void inizializzaLati(unsigned int num) {
    NumCell1Ds = num;
    Cell1DsId.resize(num);
    Cell1DsExtrema = Eigen::MatrixXi(2, num);

    for (unsigned int i = 0; i < num; ++i) {
        Cell1DsId[i] = i;
        Cell1DsExtrema(0, i) = i; // origine
        Cell1DsExtrema(1, i) = i + 1; // fine
    }
}

TEST(Cell1DsTest, Inizializzazione) {
    inizializzaLati(4);

    EXPECT_EQ(NumCell1Ds, 4);
    EXPECT_EQ(Cell1DsId.size(), 4);
    EXPECT_EQ(Cell1DsExtrema.cols(), 4);
    EXPECT_EQ(Cell1DsExtrema.rows(), 2);

    for (unsigned int i = 0; i < 4; ++i) {
        EXPECT_EQ(Cell1DsId[i], i);
        EXPECT_EQ(Cell1DsExtrema(0, i), i);
        EXPECT_EQ(Cell1DsExtrema(1, i), i + 1);
    }
}


// Test sulle facce

unsigned int NumCell2Ds = 0;
std::vector<unsigned int> Cell2DsId;
std::vector<unsigned int> Cell2DsNumVertices;
std::vector<std::vector<unsigned int>> Cell2DsVertices;
std::vector<unsigned int> Cell2DsNumEdges;
std::vector<std::vector<unsigned int>> Cell2DsEdges;

void inizializzaFacce(unsigned int num) {
    NumCell2Ds = num;
    Cell2DsId.resize(num);
    Cell2DsNumVertices.resize(num);
    Cell2DsVertices.resize(num);
    Cell2DsNumEdges.resize(num);
    Cell2DsEdges.resize(num);

    for (unsigned int i = 0; i < num; ++i) {
        Cell2DsId[i] = i;
        Cell2DsNumVertices[i] = 3 + i; // es. 3, 4, 5...
        Cell2DsNumEdges[i] = 3 + i;

        for (unsigned int j = 0; j < Cell2DsNumVertices[i]; ++j) {
            Cell2DsVertices[i].push_back(j + i); // valori fittizi
        }
        for (unsigned int j = 0; j < Cell2DsNumEdges[i]; ++j) {
            Cell2DsEdges[i].push_back(j + i); // valori fittizi
        }
    }
}

TEST(Cell2DsTest, Inizializzazione) {
    inizializzaFacce(3);

    EXPECT_EQ(NumCell2Ds, 3);
    EXPECT_EQ(Cell2DsId.size(), 3);
    EXPECT_EQ(Cell2DsNumVertices.size(), 3);
    EXPECT_EQ(Cell2DsVertices.size(), 3);
    EXPECT_EQ(Cell2DsNumEdges.size(), 3);
    EXPECT_EQ(Cell2DsEdges.size(), 3);

    for (unsigned int i = 0; i < 3; ++i) {
        EXPECT_EQ(Cell2DsId[i], i);
        EXPECT_EQ(Cell2DsVertices[i].size(), Cell2DsNumVertices[i]);
        EXPECT_EQ(Cell2DsEdges[i].size(), Cell2DsNumEdges[i]);

        for (unsigned int j = 0; j < Cell2DsNumVertices[i]; ++j) {
            EXPECT_GE(Cell2DsVertices[i][j], i);
        }
        for (unsigned int j = 0; j < Cell2DsNumEdges[i]; ++j) {
            EXPECT_GE(Cell2DsEdges[i][j], i);
        }
    }
}


// Test sui poliedri

}
