//: Playground - noun: a place where people can play

import Cocoa

// Declare constants
let GRAVITY: Float = 9.81
let ATM: Float = 1.01  // bar
let CM2MT: Float = 1000000
let KG2GR: Float = 1000
let LT2MT: Float = 0.001225
let RANGE: Float = 0.1  // % variation for range calculation

let water_density_fresh = ([4, 1000.00],
                           [5, 1000.00],
                           [10, 999.98],
                           [15, 999.20],
                           [20, 998.30],
                           [25, 997.10],
                           [30, 995.70])

let water_density_salt  = ([4, 1024.00],
                           [5, 1024.00],
                           [10,1024.98],
                           [15,1024.15],
                           [20,1024.22],
                           [25,1022.00],
                           [30,1020.56])

let body_density = ([18.5, 1.282],
                    [24.9, 1.01],
                    [25.0, 0.808])

let gas_density = ([5,  1.269],
                   [10, 1.247],
                   [15, 1.225],
                   [20, 1.204],
                   [25, 1.184],
                   [30, 1.165])


//Declare variables
var metric: Bool = true
var index: Float
var body_volume, body_dens, body_bmi: Float
var tnak_volume, tank_density: Float
var wpmm: Float     // Weight/mm in the wetsuit
var buoyancy: Float
var wdensity, tweight, exweight: Float
var pull: Float
var belt1, betl2, belt3, tbelt, belt_low, belt_high: Float


