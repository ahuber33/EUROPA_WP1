/**
 * @file Geometry.cc
 * @brief Implementation of the Geometry class for the PALLAS project.
 *
 * This file implements the methods defined in Geometry.hh to construct and
 * retrieve Geant4 geometry volumes used in the PALLAS simulation framework.
 * It provides functions to:
 *   - Load GDML-defined volumes and assign materials.
 *   - Create simplified quadrupole magnets as box volumes with vacuum material.
 *   - Construct diagnostic chambers using subtraction of cylindrical volumes.
 *
 * The Geometry class acts as a utility for building and managing
 * logical volumes in Geant4 simulations, facilitating the definition
 * of both realistic and simplified detector components.
 *
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr>
 * @date 2026
 * @copyright EUROPA Project - WP1
 */


#include "Geometry.hh"

// ***********************
// Constructor
// ***********************

/**
 * @brief Default constructor.
 *
 * Initializes internal pointers and sets up the geometry environment.
 */
Geometry::Geometry()
{
}

// ***********************
// Destructor
// ***********************

/**
 * @brief Destructor.
 *
 * Cleans up any allocated resources associated with the geometry.
 */
Geometry::~Geometry()
{
}

/**
 * @brief Load and retrieve a GDML-defined volume.
 *
 * This method uses a GDML parser to read a geometry description from a GDML file,
 * retrieve the specified volume, and assign it the provided material.
 *
 * @param path Path to the GDML file.
 * @param VName Name of the volume inside the GDML file.
 * @param material Material to assign to the retrieved logical volume.
 * @return Pointer to the created G4LogicalVolume.
 */
G4LogicalVolume* Geometry::GetGDMLVolume(const char* path, const char* VName, G4Material* material)
{
  Material = material;

  G4GDMLParser* parser = new G4GDMLParser();
  // Create tessellated volume of the requested component
  parser->Clear();
  parser->Read(G4String(path), false);
  LogicalVolume = parser->GetVolume(G4String(VName));
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


/**
 * @brief Load and retrieve a Fake Collimator volume.
 *
 * Constructs a simple box volume with the given dimensions and assigns it a material.
 * Dimensions and material will be modified after in GeometryConstruction
 *
 * @param name Name of the volume.
 * @return Pointer to the created G4LogicalVolume.
 */
G4LogicalVolume *Geometry::GetTubsVolume(G4String name, G4String material, G4double radius, G4double thickness, G4double densityfraction)
{
  Material = G4NistManager::Instance()->FindOrBuildMaterial(material);

  G4double density = Material->GetDensity() * densityfraction;

  Tubs = new G4Tubs("Tubs", 0, radius, thickness/2, 0., 360. * deg);

  if (densityfraction < 1.0)
    {
      scaledMaterial = new G4Material(
        material + "_scaled",
        density,
        Material);

        LogicalVolume = new G4LogicalVolume(Tubs, scaledMaterial, name, 0, 0, 0);
    }

  else if (densityfraction == 1.0)
    {
      LogicalVolume = new G4LogicalVolume(Tubs, Material, name, 0, 0, 0);
    }

  else
    {
      G4Exception("Geometry::GetTubsVolume",
                "InvalidDensityFraction",
                FatalException,
                "densityfraction must be <= 1.0");
    }

  return LogicalVolume;
}