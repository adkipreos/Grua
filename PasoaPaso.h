#ifndef PasoaPaso_h
#define PasoaPaso_h

//Interface
class PasoaPaso{
	public:
		PasoaPaso{int pos_final, int pin_1_motor, int pin_2_motor};
	
		void posfinal(int pos_final);

	private:
		void pasoactual(int paso_actual);
		int direccion;
		int velocidad;
		unsigned long delay_paso;
		int numero_pasos;
		
		int pin_1_motor;	
		int pin_2_motor;
};
#endif