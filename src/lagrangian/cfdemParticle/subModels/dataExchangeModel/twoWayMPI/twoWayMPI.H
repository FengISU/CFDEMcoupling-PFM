/*---------------------------------------------------------------------------*\
    CFDEMcoupling - Open Source CFD-DEM coupling

    CFDEMcoupling is part of the CFDEMproject
    www.cfdem.com
                                Christoph Goniva, christoph.goniva@cfdem.com
                                Copyright 2009-2012 JKU Linz
                                Copyright 2012-     DCS Computing GmbH, Linz
-------------------------------------------------------------------------------
License
    This file is part of CFDEMcoupling.

    CFDEMcoupling is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    CFDEMcoupling is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CFDEMcoupling; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Description
    This code is designed to realize coupled CFD-DEM simulations using LIGGGHTS
    and OpenFOAM(R). Note: this code is not part of OpenFOAM(R) (see DISCLAIMER).

    two way DEM-CFD coupling via MPI

Class
    twoWayMPI

SourceFiles
    twoWayMPI.C

\*---------------------------------------------------------------------------*/

#ifndef twoWayMPI_H
#define twoWayMPI_H

#include <mpi.h>

#include "dataExchangeModel.H"
#include "scalarIOList.H"

//=================================//
//LAMMPS/LIGGGHTS
#include <lammps.h>         // these are LAMMPS include files
#include <pair.h>
#include <force.h>
//=================================//

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class noDrag Declaration
\*---------------------------------------------------------------------------*/

class twoWayMPI
:
    public dataExchangeModel
{
private:

  // private data
    dictionary propsDict_;
    MPI_Comm comm_liggghts;

    wordList DEMVariableNames_;

    scalarIOList DEMVariables_; 

  // private member functions
    double DEMVariableValue(word);

    void updateDEMVariables();

protected:
    LAMMPS_NS::LAMMPS *lmp;

public:

    //- Runtime type information
    TypeName("twoWayMPI");


    // Constructors

        //- Construct from components
        twoWayMPI
        (
            const dictionary& dict,
            cfdemCloud& sm
        );

    // Destructor

        ~twoWayMPI();


    // Member Functions
        void getData
        (
            const word& name,
            const word& type,
            double ** const& field,
            label step
        ) const;

        void getData
        (
            const word& name,
            const word& type,
            int ** const& field,
            label step
        ) const;

        void giveData
        (
            const word& name,
            const word& type,
            double ** const& field,
            const char* datatype
        ) const;

        //============
        // double **
        void allocateArray(double**&, double, int, int) const;
        void allocateArray(double**&, double, int, const char* = "nparticles") const;
        void destroy(double**,int) const;
        //============
        // int **
        void allocateArray(int**&, int, int, int) const;
        void allocateArray(int**&, int, int, const char* = "nparticles") const;
        void destroy(int**,int) const;
        //==============
        // double *
        void destroy(double*) const;
        //============
        // int **
        void destroy(int*) const;
        //==============

        bool couple(int);

        int getNumberOfParticles() const;
        int getNumberOfClumps() const;
        int getNumberOfTypes() const;
        double* getTypeVol() const;

        scalar getCG() const { return lmp->force->cg(); }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
