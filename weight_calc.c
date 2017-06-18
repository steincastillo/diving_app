/*
 * weight_cal.c
 *
 * Copyright 2016 Stein Castillo <stein_castillo@yahoo.com>
 *
 */

#include <stdio.h>
#define TRUE 1
#define ERR_MSG "Please enter a valid number...\n"

//
//Function prototypes
//
void printheader();
float get_data(char msg[]);

///////////////////////////////////
//
// MAIN LOOP
//
///////////////////////////////////

int main()
{
    //Define data structures
    struct parameters
    {
        float body_weight;
        float body_height;
        float tsize;
        int ttype;
        float rpressure;
        int  salt_water;
        int   water_temp;
        float wetsuit;
        int  hood;
    };

    //Declare constants
    const float GRAVITY = 9.81;
    const float body_density_low = 0.781; // gr/cm3 - Overweight, BMI > 25.0
    const float body_density_norm = 1.01; // gr/cm3 - Normal weight BMI 18.5 - 24.9
    const float body_density_high = 1.282; // gr/cm3 - Fitness  BMI < 18.5
    const float atm = 1.01; //bar
    const float cm2mt = 1000000;
    const float kg2gr = 1000;
    const float lts2mt = 0.001;
    const float salt = 1024.22; // salt water density kg/mts3 @20 celsius
    const float fresh = 998.3;  //fresh water density kg/mts3 @20 celsius
    const float al_tank = 4.8; // Aluminum tank weight in kgs
    const float fe_tank = 2.6; // Steel tank weight in kgs
    const float wet_salt = 0.907; // kg * mm in the wetsuit (salt water)
    const float wet_fresh = 0.884; // kg * mm in the wetsuit (fresh water)
    const float wet_hood = 0.453; // Add 1 lbs when wearing hood
    const float RANGE = 0.1;    // 10% variation for range calculation

    //Declare variables
    int   metric; // metric or imperial?
    float body_volume, body_density;
    float body_bmi;
    float tank_volume;
    float wpmm; //Weight * mm in the wetsuit
    float bouyancy;
    float wdensity, tweight, exweight;
    float pull;
    float belt1, belt2, belt3, tbelt, belt_low, belt_high;


	//declare and initialize dive parameters
	struct parameters input;
	input.body_weight = 0;
	input.body_height = 0;
	input.tsize = 11; // liters
	input.ttype = 1; // Steel
	input.rpressure = 210; //bar
	input.salt_water = 1; //True
	input.water_temp = 20; //Celsius
	input.wetsuit = 5; //mm
	input.hood = 1; //True

	printheader();

    /////////////
	//Get diver parameters
	/////////////

	metric = get_data("Metric [1/0]: ");

	if (metric == TRUE) // Metric Units
    {
        printf("\n*** Metric Units ***\n");
        input.body_weight = get_data("Body Weight (kgs): ");    //Body weight
        input.body_height = get_data("Body Height (mts): ");    //Body height
        input.tsize = get_data("Tank size [lts]: ");            //Tank Size
        input.ttype = get_data("Steel tank [1/0]: ");           //Tank type
        input.rpressure = get_data("Rated pressure [BAR]: ");   //Rated Pressure
        input.wetsuit = get_data("Wetsuit thickness [mm]: ");   //Wetsuit
        input.hood = get_data("Wearing hood? [1/0]: ");         //Hood?
        input.salt_water = get_data("Salt water [1/0]: ");      //Salt Water?

    }
    else // Imperial Units
    {
        printf("\n*** Imperial Units ***\n");
        input.body_weight = get_data("Body Weight (lbs): ");    //Body weight
        input.body_height = get_data("Body Height (in): ");    //Body height
        input.tsize = get_data("Tank size [cft]: ");            //Tank Size
        input.ttype = get_data("Steel tank [1/0]: ");           //Tank type
        input.rpressure = get_data("Rated pressure [PSI]: ");   //Rated Pressure
        input.wetsuit = get_data("Wetsuit thickness [mm]: ");   //Wetsuit
        input.hood = get_data("Wearing hood? [1/0]: ");         //Hood?
        input.salt_water = get_data("Salt water [1/0]: ");      //Salt Water?
    }

    printf("Calculating... \n");

    body_bmi = (input.body_weight / input.body_height) / input.body_height;
    if (body_bmi > 25.0) // Determine body density
    {
        body_density = body_density_low;
    }
    else if (body_bmi < 18.5)
    {
        body_density = body_density_high;
    }
    else
    {
        body_density = body_density_norm;
    }

    if (input.salt_water == TRUE) // Salt water?
    {
        wdensity = salt;
        wpmm = wet_salt;
    }
    else
    {
        wdensity = fresh;
        wpmm = wet_fresh;
    }

    if (input.ttype == TRUE) // Steel tank?
    {
        tweight = fe_tank;
    }
    else
    {
        tweight = al_tank;
    }

    if (input.hood == TRUE) // wearing hood?
    {
        exweight = wet_hood;
    }
    else
    {
        exweight = 0;
    }

    body_volume = (input.body_weight*kg2gr)/body_density; //body volume in cm3
    body_volume = body_volume / cm2mt; // Body volume in mts3

    tank_volume = (input.rpressure * input.tsize) / atm; // tank volume in lts
    tank_volume = tank_volume * lts2mt;  // tanks volume in mts3

    bouyancy = (body_volume+tank_volume)*wdensity*GRAVITY;
    pull = (input.body_weight + tweight + input.tsize) * GRAVITY;

    belt1 = (bouyancy-pull)/(wdensity*GRAVITY);
    belt2 = input.wetsuit * wpmm;
    belt3 = exweight;
    tbelt = belt1 + belt2 + belt3;
    belt_low = tbelt - (tbelt * RANGE);
    belt_high = tbelt + (tbelt * RANGE);

	//print results
	printf("\n*** RESULTS ***\n");
	printf("Body Volume [mts3] %3.4f\n", body_volume);
	printf("Tank Volume [mts3] %3.4f\n", tank_volume);
	printf("Bouyancy [Newtons] %3.4f\n", bouyancy);
	printf("Water pull [Newtons] %3.4f\n", pull);
	printf("Weight belt 1/3 - Body & Tank [Kgs]: %3.4f\n", belt1);
	printf("Weight belt 2/3 - Wet Suit    [Kgs]: %3.4f\n", belt2);
	printf("Weight belt 3/3 - Hood        [Kgs]: %3.4f\n", belt3);
	printf("Total weight belt:            [Kgs]: %3.1f\n", tbelt);
	printf("************************************\n");
	printf("Weight belt range [Kgs] from: %3.0f to %3.0f\n", belt_low, belt_high);
    printf("\n");

	return 0;
}

/////////////////////////////////////
//
// FUNCTIONS
//
/////////////////////////////////////
void printheader(void)
{
	printf("****************************\n");
	printf("*                          *\n");
	printf("*Diving Weight CALCULATOR  *\n");
	printf("*          v1.1            *\n");
	printf("*   (Metric units only)    *\n");
	printf("*                          *\n");
	printf("****************************\n");

}

float get_data(char msg[35])
{
    float value = 0;
    char input[25];

    while (TRUE)
    {
        printf("%s", msg);
        fgets (input, 25, stdin);
        if (sscanf (input, "%f", &value) == 1) break;
        printf (ERR_MSG);
    };
    return value;
}
