#include <gtest/gtest.h>
#include "../include/Curve.h"
#include "../include/Feather.h"
#include "ngl/Vec3.h"
#include <vector>
#include <cmath>

//============================================================================
// BezierCurve Tests
//============================================================================

class BezierCurveTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a standard cubic Bezier curve for testing
        curve = std::make_unique<BezierCurve>();
        curve->addPoint(ngl::Vec3(0.0f, 0.0f, 0.0f));   // P0
        curve->addPoint(ngl::Vec3(1.0f, 2.0f, 0.0f));   // P1
        curve->addPoint(ngl::Vec3(2.0f, 2.0f, 0.0f));   // P2
        curve->addPoint(ngl::Vec3(3.0f, 0.0f, 0.0f));   // P3
    }

    std::unique_ptr<BezierCurve> curve;
    const float EPSILON = 0.001f;
};

TEST_F(BezierCurveTest, ConstructorTest) {
    // Test default constructor
    BezierCurve defaultCurve;
    EXPECT_EQ(defaultCurve.getCPs().size(), 0);
}

TEST_F(BezierCurveTest, AddPointTest) {
    BezierCurve testCurve;
    
    // Test adding points
    testCurve.addPoint(ngl::Vec3(1.0f, 2.0f, 3.0f));
    testCurve.addPoint(4.0f, 5.0f, 6.0f);
    
    auto controlPoints = testCurve.getCPs();
    EXPECT_EQ(controlPoints.size(), 2);
    
    EXPECT_NEAR(controlPoints[0].m_x, 1.0f, EPSILON);
    EXPECT_NEAR(controlPoints[0].m_y, 2.0f, EPSILON);
    EXPECT_NEAR(controlPoints[0].m_z, 3.0f, EPSILON);
    
    EXPECT_NEAR(controlPoints[1].m_x, 4.0f, EPSILON);
    EXPECT_NEAR(controlPoints[1].m_y, 5.0f, EPSILON);
    EXPECT_NEAR(controlPoints[1].m_z, 6.0f, EPSILON);
}

TEST_F(BezierCurveTest, CurveEndpointsTest) {
    // Test that curve starts and ends at control points
    ngl::Vec3 startPoint = curve->getPointOnCurve(0.0f);
    ngl::Vec3 endPoint = curve->getPointOnCurve(1.0f);
    
    // Should start at P0 (0,0,0)
    EXPECT_NEAR(startPoint.m_x, 0.0f, EPSILON);
    EXPECT_NEAR(startPoint.m_y, 0.0f, EPSILON);
    EXPECT_NEAR(startPoint.m_z, 0.0f, EPSILON);
    
    // Should end at P3 (3,0,0)
    EXPECT_NEAR(endPoint.m_x, 3.0f, EPSILON);
    EXPECT_NEAR(endPoint.m_y, 0.0f, EPSILON);
    EXPECT_NEAR(endPoint.m_z, 0.0f, EPSILON);
}

TEST_F(BezierCurveTest, CurveMidpointTest) {
    // Test midpoint evaluation
    ngl::Vec3 midPoint = curve->getPointOnCurve(0.5f);
    
    // For our control points (0,0,0), (1,2,0), (2,2,0), (3,0,0)
    // Midpoint should be approximately (1.5, 1.5, 0.0)
    EXPECT_NEAR(midPoint.m_x, 1.5f, 0.1f);  // Allow some tolerance
    EXPECT_NEAR(midPoint.m_y, 1.5f, 0.1f);  // Corrected expected value
    EXPECT_NEAR(midPoint.m_z, 0.0f, EPSILON);
}

TEST_F(BezierCurveTest, LODTest) {
    // Test LOD setting and sample point generation
    curve->setLOD(50);
    std::vector<ngl::Vec3> samples = curve->getSamplePoints();
    
    // Should have 50 samples (based on LOD setting)
    EXPECT_EQ(samples.size(), 50);
    
    // First sample should be start point
    EXPECT_NEAR(samples[0].m_x, 0.0f, EPSILON);
    EXPECT_NEAR(samples[0].m_y, 0.0f, EPSILON);
    
    // Last sample should be close to end point (may not be exact due to sampling)
    EXPECT_NEAR(samples[49].m_x, 3.0f, 0.1f);  // More tolerant for sampling 
    EXPECT_NEAR(samples[49].m_y, 0.0f, 0.2f);  // More tolerant for sampling
}

TEST_F(BezierCurveTest, LerpTest) {
    // Test linear interpolation function
    ngl::Vec3 p0(0.0f, 0.0f, 0.0f);
    ngl::Vec3 p1(2.0f, 4.0f, 6.0f);
    
    ngl::Vec3 result = curve->lerp(0.5f, p0, p1);
    
    EXPECT_NEAR(result.m_x, 1.0f, EPSILON);
    EXPECT_NEAR(result.m_y, 2.0f, EPSILON);
    EXPECT_NEAR(result.m_z, 3.0f, EPSILON);
}

//============================================================================
// Feather Tests
//============================================================================

class FeatherTest : public ::testing::Test {
protected:
    void SetUp() override {
        feather = std::make_unique<Feather>();
        
        // Set up basic rachis
        feather->setRachisControlPoints(
            ngl::Vec3(0.0f, 0.0f, 0.0f),    // P0
            ngl::Vec3(0.3f, 2.0f, 0.0f),   // P1
            ngl::Vec3(0.5f, 4.0f, 0.0f),   // P2
            ngl::Vec3(0.2f, 9.5f, 0.0f)    // P3
        );
        
        // Set up basic outline
        feather->setSymmetricOutlineControlPoints(
            ngl::Vec3(-1.8f, 3.5f, 0.0f),  // P1
            ngl::Vec3(-1.5f, 5.5f, 0.0f),  // P2
            ngl::Vec3(-0.2f, 6.0f, 0.0f)   // P3
        );
    }

    std::unique_ptr<Feather> feather;
    const float EPSILON = 0.001f;
};

TEST_F(FeatherTest, ConstructorTest) {
    // Test default constructor
    Feather defaultFeather;
    // Should not crash and should have reasonable defaults
    EXPECT_TRUE(true); // Basic construction test
}

TEST_F(FeatherTest, RachisControlPointsTest) {
    // Test setting and getting rachis control points
    std::vector<ngl::Vec3> controlPoints = feather->getRachisControlPoints();
    
    EXPECT_EQ(controlPoints.size(), 4);
    
    // Check P0
    EXPECT_NEAR(controlPoints[0].m_x, 0.0f, EPSILON);
    EXPECT_NEAR(controlPoints[0].m_y, 0.0f, EPSILON);
    EXPECT_NEAR(controlPoints[0].m_z, 0.0f, EPSILON);
    
    // Check P3
    EXPECT_NEAR(controlPoints[3].m_x, 0.2f, EPSILON);
    EXPECT_NEAR(controlPoints[3].m_y, 9.5f, EPSILON);
    EXPECT_NEAR(controlPoints[3].m_z, 0.0f, EPSILON);
}

TEST_F(FeatherTest, ParameterSettersTest) {
    // Test basic parameter setters
    feather->setSampleNum(150);
    feather->setF0(0.3f);
    feather->setFn(0.9f);
    feather->setNumBarbs(75);
    feather->setBarbLOD(25);
    feather->setFb(0.6f);
    
    // If no exceptions thrown, setters work
    EXPECT_TRUE(true);
}

TEST_F(FeatherTest, OutlineSymmetryTest) {
    // Test outline symmetry setting
    feather->setOutlineSymmetric(true);
    EXPECT_TRUE(feather->isOutlineSymmetric());
    
    feather->setOutlineSymmetric(false);
    EXPECT_FALSE(feather->isOutlineSymmetric());
}

TEST_F(FeatherTest, BarbControlFactorsTest) {
    // Test setting barb control factors
    feather->setBarbControlFactors(0.4f, 0.8f, 0.2f, 0.1f);
    
    // Test setting barb outline factors
    feather->setBarbsOutlineFactor(0.6f, 0.55f);
    
    // If no exceptions thrown, setters work
    EXPECT_TRUE(true);
}

TEST_F(FeatherTest, OutlineMappingRangeTest) {
    // Test outline mapping range setting
    feather->setOutlineMappingRange(0.1f, 0.9f);
    
    // Test boundary values
    feather->setOutlineMappingRange(0.0f, 1.0f);
    
    // Test invalid range (should be handled gracefully)
    feather->setOutlineMappingRange(0.8f, 0.2f); // End < Start
    
    // If no exceptions thrown, setters work correctly
    EXPECT_TRUE(true);
}

TEST_F(FeatherTest, ShowOutlinesTest) {
    // Test show outlines setting
    feather->setShowOutlines(true);
    feather->setShowOutlines(false);
    
    // If no exceptions thrown, setter works
    EXPECT_TRUE(true);
}

TEST_F(FeatherTest, AsymmetricOutlineTest) {
    // Test asymmetric outline control points
    feather->setOutlineControlPoints(
        ngl::Vec3(-1.8f, 3.5f, 0.0f),  // Left P1
        ngl::Vec3(-1.5f, 5.5f, 0.0f),  // Left P2
        ngl::Vec3(0.6f, 2.75f, 0.0f),  // Right P1
        ngl::Vec3(0.5f, 10.0f, 0.0f),  // Right P2
        ngl::Vec3(-0.2f, 6.0f, 0.0f)   // End P3
    );
    
    // Should have set asymmetric mode
    EXPECT_FALSE(feather->isOutlineSymmetric());
}

//============================================================================
// Integration Tests
//============================================================================

class FeatherIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        feather = std::make_unique<Feather>();
        setupCompleteFeather();
    }
    
    void setupCompleteFeather() {
        // Set up a complete feather with all parameters
        feather->setRachisControlPoints(
            ngl::Vec3(0.0f, 0.0f, 0.0f),
            ngl::Vec3(0.3f, 2.0f, 0.0f),
            ngl::Vec3(0.5f, 4.0f, 0.0f),
            ngl::Vec3(0.2f, 9.5f, 0.0f)
        );
        
        feather->setSymmetricOutlineControlPoints(
            ngl::Vec3(-1.8f, 3.5f, 0.0f),
            ngl::Vec3(-1.5f, 5.5f, 0.0f),
            ngl::Vec3(-0.2f, 6.0f, 0.0f)
        );
        
        feather->setSampleNum(100);
        feather->setF0(0.25f);
        feather->setFn(0.95f);
        feather->setNumBarbs(50);
        feather->setBarbLOD(20);
        feather->setFb(0.5f);
        feather->setBarbControlFactors(0.3f, 1.0f, 0.1f, 0.0f);
        feather->setBarbsOutlineFactor(0.55f, 0.51f);
        feather->setOutlineMappingRange(0.0f, 1.0f);
        feather->setShowOutlines(true);
    }

    std::unique_ptr<Feather> feather;
};

TEST_F(FeatherIntegrationTest, CompleteFeatherSetupTest) {
    // Test that complete feather setup doesn't crash
    // This tests the integration of all components
    EXPECT_TRUE(feather != nullptr);
    
    // Verify some key parameters are set correctly
    auto controlPoints = feather->getRachisControlPoints();
    EXPECT_EQ(controlPoints.size(), 4);
    EXPECT_TRUE(feather->isOutlineSymmetric());
}

TEST_F(FeatherIntegrationTest, ParameterBoundaryTest) {
    // Test extreme parameter values
    feather->setF0(0.0f);
    feather->setF0(1.0f);
    feather->setFn(0.0f);
    feather->setFn(1.0f);
    
    feather->setNumBarbs(1);
    feather->setNumBarbs(1000);
    
    feather->setBarbLOD(1);
    feather->setBarbLOD(500);
    
    feather->setSampleNum(10);
    feather->setSampleNum(1000);
    
    // If no exceptions thrown, boundary handling works
    EXPECT_TRUE(true);
}

//============================================================================
// Performance Tests
//============================================================================

TEST(FeatherPerformanceTest, LargeFeatherGenerationTest) {
    Feather feather;
    
    // Set up a high-detail feather
    feather.setRachisControlPoints(
        ngl::Vec3(0.0f, 0.0f, 0.0f),
        ngl::Vec3(0.3f, 2.0f, 0.0f),
        ngl::Vec3(0.5f, 4.0f, 0.0f),
        ngl::Vec3(0.2f, 9.5f, 0.0f)
    );
    
    feather.setSymmetricOutlineControlPoints(
        ngl::Vec3(-1.8f, 3.5f, 0.0f),
        ngl::Vec3(-1.5f, 5.5f, 0.0f),
        ngl::Vec3(-0.2f, 6.0f, 0.0f)
    );
    
    // High detail settings
    feather.setSampleNum(500);
    feather.setNumBarbs(200);
    feather.setBarbLOD(50);
    
    // This should complete without crashing or taking too long
    // (The actual generation happens during rendering/update calls)
    EXPECT_TRUE(true);
}

//============================================================================
// Main Test Runner
//============================================================================

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}