/*
 * ic2_master.c
 *
 *  Created on: 21 May 2026
 *      Author: Samuel
 */



#include "GAUL_drivers/i2c_master.h"
#include "GAUL_drivers/GEO_coordo.h"
#include "i2c.h"


//----------------------------------------------------
//			GESTION I2C (par DMA bouclé sauf variation des uint8_t dans la boucle main.)
//----------------------------------------------------
uint8_t statue_data_i2c = 0;	/**statue des donnée i2c. 0=chargement. A METTRE À 1 POUR DIRE READY.*/
/** statue du trensfert.
 * 0=aucun trensfer.
 * 1=en cours.
 * 2=ARRÊT. */
uint8_t statue_i2c = 2;

uint8_t statue_DMA_i2c = 0;

float tempon_i2c[3] = {0, 0, 0};


void start_i2c_loop(void){
	statue_i2c = 0;
	statue_data_i2c = 1;
}
void i2c_data_dispo(void){
	statue_data_i2c = 1;
}
void i2c_data_indispo(void){
	statue_data_i2c = 0;
}

void i2c_stop_loop(void){
	statue_i2c = 2;
}



/** sauvegarde des données les plus récentes possibles. */
void i2c_data_sender_DMA(const float val1, const float val2, const float val3){
	// si le trensfert n'as pas fini, acumule dans un autre tempon
	// ce qui permet d'envoyé la valeur la plus frèche dès qu'il a fini le trensfert.
	if(statue_i2c==1){
		tempon_i2c[0] = val1;
		tempon_i2c[1] = val2;
		tempon_i2c[2] = val3;
	}
	else{// si le trensfert est fini, alors ont peut direct trensfert vers le tempon et essayer denvoyer.
		I2C_GPS.coordo[0] = val1;
		I2C_GPS.coordo[1] = val2;
		I2C_GPS.coordo[2] = val3;
	}
}

/**trensfert les donnée pour le GPS du tempon vers le buffer pour l'envoie*/
void IC2_trensfert_tempon2bufer(void){
	for (uint8_t i = 0; i < 3; i++){
		I2C_GPS.coordo[i] = tempon_i2c[i];
	}
}

/*initier le trensfert DMA si possible.
 * */

void I2C_main_loop(const float val1, const float val2, const float val3){
	// ont place juste les données dans le tempon.

	i2c_data_sender_DMA(val1, val2, val3);
	// boucle DMA arrêté? (si ce n'ais pas le cas, le reste seras pris en charge automatiquement)
	if(statue_DMA_i2c==0){
		// ont peux recommencé ?
		if(statue_data_i2c==1 && statue_i2c==0){
			statue_DMA_i2c = 1;
			IC2_trensfert();
		}
	}
}

void IC2_trensfert(void){
	IC2_trensfert_tempon2bufer();
	statue_i2c=1;
	HAL_I2C_Master_Transmit_DMA(&hi2c1,I2C_ADRESS_SLAVE1,&I2C_GPS.I2C_REGISTERS, 12);   //TODO: verif if the size is ok.


}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {    // call when the I2C is finish.
	if (hi2c->Instance == I2C1) {
		// vérification de s'il peut envoyer.
		if(statue_data_i2c==1 && statue_i2c!=2){
			IC2_trensfert();
		}
		else{
			// s'il est bloqué, simplement mentionner qu'il est près à repartir, sans pour autant le faire.
			statue_DMA_i2c=0;
		}
		// Transfer finished! Do something here, like toggle an LED or set a flag.
	}
}


