#ifndef EUROPA_WP1EventAction_h
#define EUROPA_WP1EventAction_h 1

/**
 * @class EUROPA_WP1EventAction
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr>
 * @date 2026
 * @copyright EUROPA Project - WP1
 * @brief Header file for event-level actions in EUROPA_WP1, including per-event statistics structures.
 */

#include "G4UserEventAction.hh"

class G4Event;

/**
 * @brief Structure to store per-event input particle data
 *
 * Holds position, direction, and energy information for the particle at the start of the event.
 */
struct RunTallyInput
{
    float x = 0.0;
    float xp = 0.0;
    float y = 0.0;
    float yp = 0.0;
    float z = 0.0;
    float zp = 0.0;
    float energy = 0.0;

};


/**
 * @brief Structure for collimators statistics
 *
 * Tracks interaction coordinates, energies between primary e- and collimators.
 */
struct RunTallyConverter
{
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
    float energy = 0.0;
    bool flag =false;
    
    // Methods to set data
    void SetXInteraction(float d) { x =d; }
    void SetYInteraction(float d) { y =d; }
    void SetZInteraction(float d) { z =d; }
    void SetEnergy(float d) { energy =d; }
    void ActiveFlag(){flag=true;}
    void ResetFlag(){flag=false;}

    // Methods to access data
    float GetXInteraction() const { return x; }
    float GetYInteraction() const { return y; }
    float GetZInteraction() const { return z; }
    float GetEnergy() const { return energy; }
    bool GetFlag() {return flag;}

  
};

/**
 * @brief Structure for YAG detector statistics
 *
 * Tracks exit coordinates, particle IDs, energies, deposited energy, and flags.
 */
struct RunTallyCible
{
    std::vector<float> x_creation;
    std::vector<float> y_creation;
    std::vector<float> z_creation;
    std::vector<float> energy;

    // Methods to add data
    void AddXCreation(float d) { x_creation.push_back(d); }
    void AddYCreation(float d) { y_creation.push_back(d); }
    void AddZCreation(float d) { z_creation.push_back(d); }
    void AddEnergy(float d) { energy.push_back(d); }

    // Size accessors
    size_t XCreationSize() const { return x_creation.size(); }
    size_t YCreationSize() const { return y_creation.size(); }
    size_t ZCreationSize() const { return z_creation.size(); }
    size_t EnergySize() const { return energy.size(); }

    // Index accessors
    float GetXCreation(size_t i) const { return x_creation.at(i); }
    float GetYCreation(size_t i) const { return y_creation.at(i); }
    float GetZCreation(size_t i) const { return z_creation.at(i); }
    float GetEnergy(size_t i) const { return energy.at(i); }
};

/**
 * @brief Event action class for EUROPA_WP1
 *
 * Implements G4UserEventAction interface to handle per-event statistics,
 * including input particle, collimator, quadrupole, and detector data.
 */
class EUROPA_WP1EventAction : public G4UserEventAction
{
public:
    /** Constructor */
    EUROPA_WP1EventAction(const char *);
    
    /** Destructor */
    ~EUROPA_WP1EventAction();

    /** Called at the beginning of each event */
    void BeginOfEventAction(const G4Event *);
    
    /** Called at the end of each event */
    void EndOfEventAction(const G4Event *);


    // Setters for input particle data //
    void SetXStart(G4float d) { StatsInput.x = d; }
    void SetXpStart(G4float d) { StatsInput.xp = d; }
    void SetYStart(G4float d) { StatsInput.y = d; }
    void SetYpStart(G4float d) { StatsInput.yp = d; }
    void SetZStart(G4float d) { StatsInput.z = d; }
    void SetZpStart(G4float d) { StatsInput.zp = d; }
    void SetEnergyStart(G4float d) { StatsInput.energy = d; }

    // Methods to access data
    float GetXStart() {return StatsInput.x;}
    float GetXpStart() {return StatsInput.xp;}
    float GetYStart() {return StatsInput.y;}
    float GetYpStart() {return StatsInput.yp;}
    float GetZStart() {return StatsInput.z;}
    float GetZpStart() {return StatsInput.zp;}
    float GetEnergyStart() {return StatsInput.energy;}

    /** Accessors for generic detector statistics */
    RunTallyConverter& GetConverter() { return StatsConverter; }

    /** Accessors for generic detector statistics */
    RunTallyCible& GetCible() { return StatsCible; }

private:
    TTree *EventTree;                        ///< ROOT tree for per-event data
    TBranch *EventBranch;                    ///< ROOT branch for event tree
    RunTallyInput StatsInput;                ///< Input particle statistics
    RunTallyConverter StatsConverter; ///< Horizontal converter statistics
    RunTallyCible StatsCible;                  ///< Beam Stop Cible statistics
    G4String suffixe;                        ///< Suffix for output naming
};

#endif
