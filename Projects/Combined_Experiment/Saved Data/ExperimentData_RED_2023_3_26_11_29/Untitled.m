relative_pose_inertial = [dataClass.BLACK_Px_m - dataClass.RED_Px_m, dataClass.BLACK_Py_m - dataClass.RED_Py_m, -(dataClass.BLACK_Rz_m + dataClass.RED_Rz_m)];
 

SPOTNet_target_x_inertial = dataClass.RED_Px_m + dataClass.ZED_Relative_X_m.*cos(dataClass.RED_Rz_m) - dataClass.ZED_Relative_Y_m.*sin(dataClass.RED_Rz_m);
SPOTNet_target_y_inertial = dataClass.RED_Py_m + dataClass.ZED_Relative_X_m.*sin(dataClass.RED_Rz_m) + dataClass.ZED_Relative_Y_m.*cos(dataClass.RED_Rz_m);
SPOTNet_target_angle_inertial = dataClass.RED_Rz_m + dataClass.ZED_Relative_Yaw_rad;

relative_pose_inertial_ZED = [SPOTNet_target_x_inertial - dataClass.RED_Px_m, SPOTNet_target_y_inertial - dataClass.RED_Py_m, SPOTNet_target_angle_inertial - dataClass.RED_Rz_m];     

plot(relative_pose_inertial(:,3))
hold on
plot(relative_pose_inertial_ZED(:,3))
yyaxis right
plot(dataClass.ZED_Confidence)
