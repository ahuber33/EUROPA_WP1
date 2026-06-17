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

  /** @brief Construct Convertor. */
  void ConstructConvertor();

  /** @brief Construct Cible. */
  void ConstructCible();


  /** @brief Construct method required by Geant4 kernel. */
  G4VPhysicalVolume *Construct() override;

  /** @brief Print geometry parameters. */
  static void Print();
  ///@}


  /** @name Geometry Parameters */
  ///@{
  void SetSpectrumTemperature(const G4double temp) {fSpectrumTemperature = temp;};
  void SetSpectrumEcut(const G4double cut) {fSpectrumEcut = cut;};
  void SetConvertorMaterial(const std::string material) {fConvertorMaterial = material;};
  void SetConvertorRadius(const G4double radius) {fConvertorRadius = radius;};
  void SetConvertorThickness(const G4double thickness) {fConvertorThickness = thickness;};
  void SetCibleRadius(const G4double radius) {fCibleRadius = radius;};
  void SetCibleThickness(const G4double thickness) {fCibleThickness = thickness;};
  void SetCibleDensityFraction(const G4double density) {fCibleDensityFraction = density;};

  const float GetSpectrumTemperature() const {return fSpectrumTemperature;}
  const float GetSpectrumEcut() const {return fSpectrumEcut;}
  const std::string GetConvertorMaterial() const {return fConvertorMaterial;}
  const float GetConvertorRadius() const {return fConvertorRadius;}
  const float GetConvertorThickness() const {return fConvertorThickness;}
  const float GetCibleRadius() const {return fCibleRadius;}
  const float GetCibleThickness() const {return fCibleThickness;}
  const float GetCibleDensityFraction() const {return fCibleDensityFraction;}
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
  G4double fSpectrumTemperature = 3*CLHEP::MeV;
  G4double fSpectrumEcut = 200*CLHEP::MeV;
  std::string fConvertorMaterial = "G4_W";
  G4double fConvertorRadius = 1*CLHEP::mm;
  G4double fConvertorThickness = 1*CLHEP::mm;
  G4double fCibleRadius = 1*CLHEP::mm;
  G4double fCibleThickness = 1*CLHEP::mm;
  G4double fCibleDensityFraction = 1;

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
  G4LogicalVolume *LogicalConvertor=nullptr;
  G4LogicalVolume *LogicalCible=nullptr;

  /** @brief Physical volumes (placements in space). */
  G4VPhysicalVolume *PhysicalWorld=nullptr;
  G4VPhysicalVolume *PhysicalHolder=nullptr;
  G4VPhysicalVolume *PhysicalConvertor=nullptr;
  G4VPhysicalVolume *PhysicalCible=nullptr;

  /** @brief Rotation matrices for component orientation. */
  G4RotationMatrix DontRotate;
  G4RotationMatrix Flip;
  G4RotationMatrix* RotationMatrix;

};
#endif
