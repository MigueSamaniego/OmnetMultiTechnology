//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package src.node_bus.heterogeneous_scenario;


import simulations.BusCosenza.cars.Highway;
import src.node_bus.access_point.APwifi;

import inet.node.ethernet.Eth10G;


network Heterogeneous extends Highway
{	parameters:
    	int numWlanInterfaces = default(0);
    
    submodules:
//        ap[0]: APwifi {
//            parameters:
//                numWlanInterfaces = 1;
//        	    numPppInterfaces  = 1;
//        	    numEthInterfaces  = 0;
//                @display("p=555,258;i=veins/sign/yellowdiamond;is=vs");
//        }
	
	connections allowunconnected:
        
//        router.pppg++ <--> Eth10G <--> ap[0].pppg++;

}
