/**
 * @file EUROPA_WP1GeometryConstruction.cc
 * @brief Implements the detector geometry and magnetic field setup for the PALLAS simulation.
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr>
 * @date 2026
 * @copyright EUROPA Project - WP1
 */

#include "EUROPA_WP1GeometryConstruction.hh"

using namespace CLHEP;

/**
 * @brief Constructor for EUROPA_WP1GeometryConstruction.
 *
 * Initializes the base geometry and attaches the geometry messenger
 * for interactive user control via macro commands.
 */
EUROPA_WP1GeometryConstruction::EUROPA_WP1GeometryConstruction()
    : G4VUserDetectorConstruction()
{
    Geom = std::make_unique<Geometry>();
    fGeometryMessenger = std::make_unique<EUROPA_WP1GeometryMessenger>(this);
}

/**
 * @brief Destructor for EUROPA_WP1GeometryConstruction.
 */
EUROPA_WP1GeometryConstruction::~EUROPA_WP1GeometryConstruction() = default;

/**
 * @brief Print a summary of the current geometry setup.
 */
void EUROPA_WP1GeometryConstruction::Print()
{
    G4cout << "\n------------------------------------------------------" << G4endl;
    G4cout << "-----------------------------------------------------" << G4endl;
}

/**
 * @brief Assign a visualization color to a logical volume.
 *
 * Predefined colors can be applied to logical volumes for debugging or visualization.
 *
 * @param LogicalVolume Pointer to the logical volume to colorize.
 * @param Color Name of the color (e.g. "red", "green", "gray").
 */
void EUROPA_WP1GeometryConstruction::SetLogicalVolumeColor(
    G4LogicalVolume *LogicalVolume, G4String Color)
{
    // ***********************
    // Visualization Colors
    // ***********************
    invis = new G4VisAttributes(G4Colour(255 / 255., 255 / 255., 255 / 255.));
    invis->SetVisibility(false);

    black = new G4VisAttributes(G4Colour(0, 0, 0, 0.9));
    black->SetForceSolid(true);
    black->SetVisibility(true);

    white = new G4VisAttributes(G4Colour(1, 1, 1, 1.));
    white->SetForceSolid(true);
    white->SetVisibility(true);

    gray = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.5));
    gray->SetVisibility(true);

    red = new G4VisAttributes(G4Colour(1, 0, 0, 0.3));
    red->SetForceSolid(true);
    red->SetVisibility(true);

    orange = new G4VisAttributes(G4Colour(1, 0.5, 0, 0.3));
    orange->SetForceSolid(true);
    orange->SetVisibility(true);

    yellow = new G4VisAttributes(G4Colour(1, 1, 0, 0.5));
    yellow->SetForceSolid(true);
    yellow->SetVisibility(true);

    green = new G4VisAttributes(G4Colour(0, 1, 0, 0.3));
    green->SetForceSolid(true);
    green->SetVisibility(true);

    cyan = new G4VisAttributes(G4Colour(0, 1, 1, 0.3));
    cyan->SetForceSolid(true);
    cyan->SetVisibility(true);

    blue = new G4VisAttributes(G4Colour(0, 0, 1, 0.5));
    blue->SetForceSolid(true);
    blue->SetVisibility(true);

    magenta = new G4VisAttributes(G4Colour(1, 0, 1, 0.3));
    magenta->SetVisibility(true);

    // Apply selected color
    if (Color == "invis")
    {
        LogicalVolume->SetVisAttributes(invis);
    }
    else if (Color == "black")
    {
        LogicalVolume->SetVisAttributes(black);
    }
    else if (Color == "white")
    {
        LogicalVolume->SetVisAttributes(white);
    }
    else if (Color == "gray")
    {
        LogicalVolume->SetVisAttributes(gray);
    }
    else if (Color == "red")
    {
        LogicalVolume->SetVisAttributes(red);
    }
    else if (Color == "orange")
    {
        LogicalVolume->SetVisAttributes(orange);
    }
    else if (Color == "yellow")
    {
        LogicalVolume->SetVisAttributes(yellow);
    }
    else if (Color == "green")
    {
        LogicalVolume->SetVisAttributes(green);
    }
    else if (Color == "cyan")
    {
        LogicalVolume->SetVisAttributes(cyan);
    }
    else if (Color == "blue")
    {
        LogicalVolume->SetVisAttributes(blue);
    }
    else if (Color == "magenta")
    {
        LogicalVolume->SetVisAttributes(magenta);
    }
}

/**
 * @brief Create the world and holder volumes.
 *
 * Defines the simulation world as a large vacuum box and
 * places a holder volume inside it for containing components.
 */
void EUROPA_WP1GeometryConstruction::CreateWorldAndHolder()
{
    Vacuum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

    G4Box *SolidWorld = new G4Box("SolidWorld", 2.1 * m, 15.1 * m, 2.1 * m);
    LogicalWorld = new G4LogicalVolume(SolidWorld, Vacuum, "LogicalWorld");
    SetLogicalVolumeColor(LogicalWorld, "invis");

    G4Box *SolidHolder = new G4Box("SolidHolder", 2.05 * m, 15.05 * m, 2.05 * m);
    LogicalHolder = new G4LogicalVolume(SolidHolder, Vacuum, "LogicalHolder");
    SetLogicalVolumeColor(LogicalHolder, "invis");

    PhysicalWorld = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0, 0, 0)),
                                      "World", LogicalWorld, NULL, false, 0);

    PhysicalHolder = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0, 0, 0)),
                                       LogicalHolder, "Holder", LogicalWorld, false, 0);
}


void EUROPA_WP1GeometryConstruction::ConstructFakeVerticalCollimator()
{
    LogicalPALLAS_Collimator_V = Geom->GetFakeCollimatorVolume("V");
    Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");
    LogicalPALLAS_Collimator_V->SetMaterial(Material);

    G4GeometryManager::GetInstance()->OpenGeometry();

    auto pipe1 = static_cast<G4Box *>(G4LogicalVolumeStore::GetInstance()->GetVolume("V")->GetSolid());
    pipe1->SetXHalfLength(40 / 2);
    pipe1->SetYHalfLength(120 / 2);
    pipe1->SetZHalfLength(40 / 2);

    SetLogicalVolumeColor(LogicalPALLAS_Collimator_V, "blue");

    G4RotationMatrix *rotationMatrix = new G4RotationMatrix();
    rotationMatrix->rotateX(0.0 * deg);

    G4ThreeVector translation1((-fVerticalSlitOpening/2 - 40/2 * mm), fSourceCollimatorsDistance + 123, 0.0 * mm);

    G4ThreeVector translation2((fVerticalSlitOpening/2 + 40/2 * mm), fSourceCollimatorsDistance + 123, 0.0 * mm);

    PhysicalPALLAS_FakeCollimator_V1 = new G4PVPlacement(rotationMatrix, translation1, LogicalPALLAS_Collimator_V, "VerticalCollimator", LogicalHolder, false, 0);
    PhysicalPALLAS_FakeCollimator_V2 = new G4PVPlacement(rotationMatrix, translation2, LogicalPALLAS_Collimator_V, "VerticalCollimator", LogicalHolder, false, 0);
}

void EUROPA_WP1GeometryConstruction::ConstructFakeHorizontalCollimator()
{

    LogicalPALLAS_Collimator_H = Geom->GetFakeCollimatorVolume("H");
    Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");
    LogicalPALLAS_Collimator_H->SetMaterial(Material);

    G4GeometryManager::GetInstance()->OpenGeometry();

    auto pipe1 = static_cast<G4Box *>(G4LogicalVolumeStore::GetInstance()->GetVolume("H")->GetSolid());
    pipe1->SetXHalfLength(40 / 2);
    pipe1->SetYHalfLength(60 / 2);
    pipe1->SetZHalfLength(40 / 2);

    SetLogicalVolumeColor(LogicalPALLAS_Collimator_H, "black");

    G4RotationMatrix *rotationMatrix = new G4RotationMatrix();
    rotationMatrix->rotateX(0.0 * deg);

    G4ThreeVector translation1(0.0 * mm, fSourceCollimatorsDistance + 30, (-fHorizontalSlitOpening/2 - 40/2 * mm));

    G4ThreeVector translation2(0.0 * mm, fSourceCollimatorsDistance + 30, (fHorizontalSlitOpening/2 + 40/2 * mm));

    PhysicalPALLAS_FakeCollimator_H1 = new G4PVPlacement(rotationMatrix, translation1, LogicalPALLAS_Collimator_H, "HorizontalCollimator", LogicalHolder, false, 0);
    PhysicalPALLAS_FakeCollimator_H2 = new G4PVPlacement(rotationMatrix, translation2, LogicalPALLAS_Collimator_H, "HorizontalCollimator", LogicalHolder, false, 0);

    }


    /**
     * @brief Construct the full detector geometry for the simulation.
     *
     * This method initializes and builds all the physical volumes required
     * for the simulation. It cleans up any previously defined geometry,
     * applies common rotation matrices, and constructs the detector
     * components according to display flags.
     *
     * Steps performed:
     * - Clean existing geometry and volume stores to avoid duplication.
     * - Define common rotation matrices used for detector components.
     * - Create the world volume and geometry holder.
     * - Construct quadrupoles volume.
     * - Build either the full or simplified PALLAS geometry depending
     *   on configuration flags.
     * - Optionally construct collimators and quadrupoles if enabled.
     * - Return the fully initialized world volume.
     *
     * @return Pointer to the top-level physical volume (`PhysicalWorld`)
     *         containing the entire detector setup.
     */
    G4VPhysicalVolume *EUROPA_WP1GeometryConstruction::Construct()
    {
        // --- Cleanup of previous geometry ----------------------------------------
        G4GeometryManager::GetInstance()->OpenGeometry();
        G4PhysicalVolumeStore::GetInstance()->Clean();
        G4LogicalVolumeStore::GetInstance()->Clean();
        G4SolidStore::GetInstance()->Clean();

        // --- Define common rotation matrices -------------------------------------
        DontRotate.rotateX(0.0 * deg);
        Flip.rotateZ(0 * deg);
        Flip.rotateX(90 * deg);
        Flip.rotateY(0 * deg);

        // #########################################################################
        //  DEFINE GEOMETRY VOLUMES
        // #########################################################################

        /// Create the world and main holder volume
        CreateWorldAndHolder();
        ConstructFakeHorizontalCollimator();
        ConstructFakeVerticalCollimator();

        G4cout << "END OF THE DETECTOR CONSTRUCTION" << G4endl;

        // --- Return the fully constructed world volume ---------------------------
        return PhysicalWorld;
    }
