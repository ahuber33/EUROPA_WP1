#ifndef EUROPA_WP1GeometryConstruction_h
#define EUROPA_WP1GeometryConstruction_h 1

/**
 * @class EUROPA_WP1GeometryConstruction
 * @brief Defines the detector geometry and magnetic fields for the PALLAS simulation.
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr>
 * @date 2026
 * @copyright EUROPA Project - WP1
 *
 * This class is responsible for:
 *  - Building the world and detector components.
 *  - Setting visualization attributes.
 *  - Defining quadrupoles, collimators, and experimental sections.
 *  - Handling field configurations (quadrupole gradients, dipole fields).
 *  - Providing user control over geometry display.
 */

#include "Geometry.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "EUROPA_WP1GeometryMessenger.hh"

class Geometry;
class EUROPA_WP1GeometryMessenger;


class EUROPA_WP1GeometryConstruction final : public G4VUserDetectorConstruction
{
public:
  /** @brief Default constructor. */
  EUROPA_WP1GeometryConstruction();

  /** @brief Destructor. */
  ~EUROPA_WP1GeometryConstruction() override;

  /** @name Geometry Construction */
  ///@{
  /** @brief Set color for a given logical volume. */
  void SetLogicalVolumeColor(G4LogicalVolume* LogicalVolume, G4String color);

  /** @brief Create world and base holder. */
  void CreateWorldAndHolder();

  /** @brief Construct Vertical collimators with slit opening modifiable. */
  void ConstructFakeVerticalCollimator();

  /** @brief Construct Horizontal collimators with slit opening modifiable. */
  void ConstructFakeHorizontalCollimator();


  /** @brief Construct method required by Geant4 kernel. */
  G4VPhysicalVolume *Construct() override;

  /** @brief Print geometry parameters. */
  static void Print();
  ///@}


  /** @name Geometry Parameters */
  ///@{
  void SetQ1Length(const G4double QLength) {fQ1Length = QLength;};
  void SetQ2Length(const G4double QLength) {fQ2Length = QLength;};
  void SetQ3Length(const G4double QLength) {fQ3Length = QLength;};
  void SetQ4Length(const G4double QLength) {fQ4Length = QLength;};
  void SetSourceQ1Distance(const G4double distance) {fSourceQ1Distance = distance;};
  void SetQ1Q2Distance(const G4double distance) {fQ1Q2Distance = distance;};
  void SetQ2Q3Distance(const G4double distance) {fQ2Q3Distance = distance;};
  void SetQ3Q4Distance(const G4double distance) {fQ3Q4Distance = distance;};
  void SetSourceCollimatorsDistance(const G4double distance) {fSourceCollimatorsDistance = distance;};
  void SetVerticalSlitOpening(const G4double opening) {fVerticalSlitOpening = opening;};
  void SetHorizontalSlitOpening(const G4double opening) {fHorizontalSlitOpening = opening;};

  const float GetQ1Length() const {return fQ1Length;}
  const float GetQ2Length() const {return fQ2Length;}
  const float GetQ3Length() const {return fQ3Length;}
  const float GetQ4Length() const {return fQ4Length;}
  const float GetSourceQ1Distance() const {return fSourceQ1Distance;}
  const float GetQ1Q2Distance() const {return fQ1Q2Distance;}
  const float GetQ2Q3Distance() const {return fQ2Q3Distance;}
  const float GetQ3Q4Distance() const {return fQ3Q4Distance;}
  const float GetSourceCollimatorsDistance() const {return fSourceCollimatorsDistance;}
  const float GetVerticalSlitOpening() const {return fVerticalSlitOpening;}
  const float GetHorizontalSlitOpening() const {return fHorizontalSlitOpening;}
  ///@}


private:
  /** @brief Geometry handler. */
  std::unique_ptr<Geometry> Geom;

    /** @brief UI messenger for runtime commands. */
  std::unique_ptr<EUROPA_WP1GeometryMessenger> fGeometryMessenger;

  /** @brief Default materials. */
  G4Material *Vacuum;
  G4Material *Material;


  /** @brief Default magnetic and geometry values. */
  G4double fQ1Length = 0.1*CLHEP::m;
  G4double fQ2Length = 0.1*CLHEP::m;
  G4double fQ3Length = 0.2*CLHEP::m;
  G4double fQ4Length = 0.1*CLHEP::m;
  G4double fSourceQ1Distance =0.152*CLHEP::m;
  G4double fQ1Q2Distance = 0.168*CLHEP::m;
  G4double fQ2Q3Distance = 0.23*CLHEP::m;
  G4double fQ3Q4Distance = 0.751*CLHEP::m;
  G4double fSourceCollimatorsDistance = 3000*CLHEP::mm;
  G4double fVerticalSlitOpening = 60*CLHEP::mm;
  G4double fHorizontalSlitOpening = 120*CLHEP::mm;

  /** @brief Visualization attributes (colors). */
  G4VisAttributes *invis = nullptr; // init all the pointers
  G4VisAttributes *white = nullptr;
  G4VisAttributes *gray = nullptr;
  G4VisAttributes *black = nullptr;
  G4VisAttributes *red = nullptr;
  G4VisAttributes *orange = nullptr;
  G4VisAttributes *yellow = nullptr;
  G4VisAttributes *green = nullptr;
  G4VisAttributes *cyan = nullptr;
  G4VisAttributes *blue = nullptr;
  G4VisAttributes *magenta = nullptr;

  /** @brief Logical volumes (geometry definitions). */
  G4LogicalVolume *LogicalWorld=nullptr;
  G4LogicalVolume *LogicalHolder=nullptr;
  G4LogicalVolume *LogicalPALLAS_Collimator_H1=nullptr;
  G4LogicalVolume *LogicalPALLAS_Collimator_H2=nullptr;
  G4LogicalVolume *LogicalPALLAS_Collimator_H=nullptr;
  G4LogicalVolume *LogicalPALLAS_Collimator_V1=nullptr;
  G4LogicalVolume *LogicalPALLAS_Collimator_V2=nullptr;
  G4LogicalVolume *LogicalPALLAS_Collimator_V=nullptr;

  /** @brief Physical volumes (placements in space). */
  G4VPhysicalVolume *PhysicalWorld=nullptr;
  G4VPhysicalVolume *PhysicalHolder=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_FakeCollimator_H1=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_FakeCollimator_H2=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_FakeCollimator_V1=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_FakeCollimator_V2=nullptr;

  /** @brief Rotation matrices for component orientation. */
  G4RotationMatrix DontRotate;
  G4RotationMatrix Flip;
  G4RotationMatrix* RotationMatrix;

};
#endif
