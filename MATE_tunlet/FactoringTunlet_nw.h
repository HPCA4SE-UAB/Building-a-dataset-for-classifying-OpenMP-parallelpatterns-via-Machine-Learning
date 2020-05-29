/**************************************************************************
*                    Universitat Autonoma de Barcelona,					  *
*              HPC4SE: http://grupsderecerca.uab.cat/hpca4se/             *
*                        Analysis and Tuning Group, 					  *
*					            2002 - 2018                  			  */
/**************************************************************************
*	  See the LICENSE.md file in the base directory for more details      *
*									-- 									  *
*	This file is part of MATE.											  *	
*																		  *
*	MATE is free software: you can redistribute it and/or modify		  *
*	it under the terms of the GNU General Public License as published by  *
*	the Free Software Foundation, either version 3 of the License, or     *
*	(at your option) any later version.									  *
*																		  *
*	MATE is distributed in the hope that it will be useful,				  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 		  *
*	GNU General Public License for more details.						  *
*																		  *
*	You should have received a copy of the GNU General Public License     *
*	along with MATE.  If not, see <http://www.gnu.org/licenses/>.         *
*																		  *
***************************************************************************/

#if !defined FACTORINGTUNLET_H
#define FACTORINGTUNLET_H

//----------------------------------------------------------------------
//
// FactoringTunlet.h
// 
//
// Anna Morajko, UAB, 2004
//
//----------------------------------------------------------------------

#include "AppModel.h" 
#include "FactoringStats_nw.h" 
//#include "FactoringOptimizer.h" 
#include <map>
#include "Config.h"

using namespace Model;

// Analyzer process:
//	 create DTAPI, initialize collector, etc.
//	 create application model
//	 initialize all tunlets
//	 start application
//	 handle events....
//	 destroy tunlets
//	 destroy app model
//	 fin

static char * DefConfigFile = "Tunlet.ini";

class Tunlet {
	public:

		/**
		 * @brief Initializes the tunlet
		 * @param app App associated to the tunlet
		 */
		virtual void Initialize (Model::Application & app) = 0;

		/**
		 * @brief Asserts that _app != 0 and sets the task handler of the app to the current one
		 */
		virtual void BeforeAppStart () {}

		/**
		 * @brief Sets tl=0.3 and lambda=0.5
		 */
		virtual void AppStarted () {}

		/**
		 * @brief Sets _app = 0
		 */
		virtual void Destroy () = 0;
};


enum FactoringEvent
{ 
	idCopy = 1,
	idCopy_End = 2,
	idScale = 3,
	idScale_End = 4,
	idAdd = 5,
	idAdd_End = 6,
	idTriad = 7,
	idTriad_End = 8,
	idReduction = 9,
	idReduction_End = 10,
	id2PStencil = 11,
	id2PStencil_End = 12,
	id2D4PStencil = 13,
	id2D4PStencil_End = 14,
	idGather = 15,
	idGather_End = 16,
	idScatter = 17,
	idScatter_End = 18,
	idStride2 = 19,
	idStride2_End = 20,
	idStride4 = 21,
	idStride4_End = 22,
	idStride16 = 23,
	idStride16_End = 24,
	idStride64 = 25,
	idStride64_End = 26,
	idRows = 27,
	idRows_End = 28,
	idTest = 29,
	idTest_End = 30
};

/**
 * @struct Stats
 * @brief Struct that stores the mean and standard deviation values
 */
struct Stats
{
	/**
	 * @brief Standard deviation attribute
	 */
    double desv;

	/**
	 * @brief Mean attribute
	 */
    double mean;       
};

/**
 * @brief Window that will store statistics of the workers
 */
struct Ventana
{
    /**
     * @brief Size of the window
     */
    int TAM;

    /**
     * @brief Pointer to a set of stats like the mean, std deviation etc.
     */
    Stats * historico;       
};
/**
 * @brief Factoring optimization tunlet for m/w apps.
 *
 */
class FactoringTunlet : public Tunlet, public Model::EventHandler,
						public Model::TaskHandler {
	// maps iteration index to iteration data
	typedef std::map<int, IterData *> IterMap;
	
	public:
		/**
		 *
		 */
		FactoringTunlet();

		/**
		 *
		 */
		~FactoringTunlet();

		/**
		 * @param app
		 */
		void Initialize(Model::Application & app);

		/**
		 *
		 */
		void BeforeAppStart();

		/**
		 *
		 */
		void Destroy();

		/**
		 * @brief returns event name
		 * @param value
		 */
		std::string GetFuncName(int event_id);
		/**
		 * @brief handles all incoming events
		 * @param r
		 */
		void HandleEvent(Model::EventRecord const & r);

		/**
		 * @param t
		 */
		void TaskStarted(Model::Task & t);

		/**
		 * @param t
		 */
		void TaskTerminated(Model::Task & t);

		/**
		 * @brief The configure of the events are read from tunlet.ini and after, the events are created
		 * with that configure.
		 */
		void CreateEvent();

		/**
		 * @brief The configure of the events are read from tunlet.ini and after, the events are created
		 * with that configure.
		 */
		void CreateEvent(Model::Task & t);

	private:

		void TestEvent (Event e);
		/**
		 * @param t
		 */
		void InsertWorkerEvents(Model::Task & t);

		/**
		 * @param t
		 */
		void InsertMasterEvents(Model::Task & t);

		/**
		 * @param iterIdx
		 */
		IterData * FindIterData(int iterIdx);

		/**
		 * @param numworker
		 */
		float CalculateFactorZero(int numworker);

		/**
		 * @param numworker
		 */
		float CalculateFactorRest(int numworker);
		//int GetNW(){ return _nw;};

		/**
		 * @param nw
		 * @param V
		 * @param sendW
		 * @param alfa
		 * @param Tc
		 */
		double CalculatePi(int nw, int V, int sendW, double alfa, double Tc);

		/**
		 * @param data
		 */
		void Tuning_NW(IterData * data);

		/**
		 * @param iterIdx
		 * @param numworkers
		 */
		void Tuning_LB(int iterIdx, int numworkers);

		/**
		 * @param iterIdx
		 */
		void TryTuning(int iterIdx);

		/**
		 *
		 * @param iterIdx
		 * @param batchIdx
		 */
		void TryTuning(int iterIdx, int batchIdx);

	private:
		Model::Application * _app;
		int                  _nw; //Number of workers
		double               _M0; //Latency of the network
		double               _LAMBDA; //Costs of sending 1 byte
		IterMap			 	 _iterMap;
		int					 _lastIterIdx; // indicates current iteration
										   //index on master
		Ventana              _ventana;
		//FactoringOptimizer	 _optimizer; //????
		//float				 _toleranceF; //?????
		Config				 _cfg;
		
		ofstream fs;
		std::string data_file = "data.csv";
		std::vector<std::string> listPAPI;
};
#endif // of FACTORINGTUNLET_H
