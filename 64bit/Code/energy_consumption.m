%************************************************************************************
% Copyright (C) 2016                                                               %
% TETCOS, Bangalore. India                                                         %
%                                                                                  %
% Tetcos owns the intellectual property rights in the Product and its content.     %
% The copying, redistribution, reselling or publication of any or all of the       %
% Product or its content without express prior written consent of Tetcos is        %
% prohibited. Ownership and / or any other right relating to the software and all  %
% intellectual property rights therein shall remain at all times with Tetcos.      %
%                                                                                  %
% Author: 	Dhruvang														       %
%                                                                                  %
% ---------------------------------------------------------------------------------%

function energy_consumption(x,power)
   
% 3D graph plotting starts here

new_p = power;
tri = delaunay(x(:,1),x(:,2));
h = trisurf(tri,x(:,1),x(:,2),new_p);
%axis vis3d
min_p = min(new_p);
max_p = max(new_p);
z_min = round(2*min_p - max_p);
z_max = round(2*max_p - min_p);
x_max = max(x(:,1));
y_max = max(x(:,2));
axis([0 x_max 0 y_max 0 5000]); 
xlabel('Sensor X position') 
ylabel('Sensor Y position') 
zlabel('Energy Consumed (mJ)')
lighting phong
shading interp
colorbar EastOutside


% 3D graph plotting ends here


end


