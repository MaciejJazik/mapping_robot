
"use strict";

let GetPlan = require('./GetPlan.js')
let PublishMap = require('./PublishMap.js')
let SetGoal = require('./SetGoal.js')
let ListLabels = require('./ListLabels.js')
let SetLabel = require('./SetLabel.js')
let GetMap = require('./GetMap.js')
let ResetPose = require('./ResetPose.js')

module.exports = {
  GetPlan: GetPlan,
  PublishMap: PublishMap,
  SetGoal: SetGoal,
  ListLabels: ListLabels,
  SetLabel: SetLabel,
  GetMap: GetMap,
  ResetPose: ResetPose,
};
