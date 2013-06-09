

//Format: [quad #] [<lower left || top left || top right>] [x, y, z]
//Note: the y and z coords might be flipped as the source engine might have the y axis as the z and vice-versa :|

//also assign each vert a color, in my opinion.

//BIG WARNING! BIG WARNING! BIG WARNING! BIG WARNING! BIG WARNING! BIG WARNING! BIG WARNING! BIG WARNING! BIG WARNING! 
//TEX_TIRE_FRONT_BACK would be best declared as a triangle fan more than anything, as it's the top of a cylinder. It is rendered in the source SDK as a triangle fan.
//So, for safety, we should treat those respective surfaces as triangle fans.

int front_back_indices[] = {
15,
16,
25,
26,
35,
44,
45,
46};

const size_t car_points_length = 54;

bool in_fbi(int index)
{
    for (int i = 0; i < 8; ++i)
        if (front_back_indices[i] == index)
            return true;
    return false;
}

float car_points [][3][3] = 

{
{{-608, 396, 40}, {-608, 416, 38}, {-584, 416, 38}}, 
// TEX_CAR_SIDE

{{-608, 422, 32}, {-608, 416, 38}, {-608, 396, 40}}, 
// TEX_CAR_SIDE

{{-584, 416, 38}, {-584, 422, 32}, {-584, 390, 32}}, 
// TEX_CAR_SIDE

{{-584, 422, 32}, {-584, 416, 38}, {-608, 416, 38}}, 
// TEX_GLASS_WINDOW

{{-584, 396, 40}, {-584, 390, 32}, {-608, 390, 32}}, 
// TEX_CAR_SIDE

{{-608, 390, 32}, {-608, 388, 26}, {-608, 432, 26}}, 
// TEX_CAR_SIDE

{{-584, 388, 26}, {-584, 390, 32}, {-584, 422, 32}}, 
// TEX_CAR_SIDE

{{-608, 422, 32}, {-608, 432, 26}, {-584, 432, 26}}, 
// TEX_CAR_SIDE

{{-608, 388, 26}, {-608, 390, 32}, {-584, 390, 32}}, 
// TEX_CAR_SIDE

{{-584, 432, 20}, {-608, 432, 20}, {-608, 388, 20}}, 
// TEX_CAR_SIDE

{{-608, 388, 20}, {-608, 432, 20}, {-608, 432, 26}}, 
// TEX_CAR_SIDE

{{-584, 388, 26}, {-584, 432, 26}, {-584, 432, 20}}, 
// TEX_CAR_SIDE

{{-608, 432, 20}, {-584, 432, 20}, {-584, 432, 26}}, 
// TEX_CAR_SIDE

{{-608, 388, 26}, {-584, 388, 26}, {-584, 388, 20}}, 
// TEX_CAR_SIDE

{{-606, 417, 17}, {-606, 416, 20}, {-606, 417, 23}}, 
// TEX_TIRE_FRONT_BACK

{{-610, 420, 24}, {-610, 417, 23}, {-610, 416, 20}}, 
// TEX_TIRE_FRONT_BACK

{{-610, 423, 23}, {-610, 424, 20}, {-606, 424, 20}},
// TEX_TIRE_SIDE

{{-610, 420, 24}, {-610, 423, 23}, {-606, 423, 23}},
// TEX_TIRE_SIDE

{{-610, 417, 23}, {-610, 420, 24}, {-606, 420, 24}},
// TEX_TIRE_SIDE

{{-610, 416, 20}, {-610, 417, 23}, {-606, 417, 23}},
// TEX_TIRE_SIDE

{{-610, 417, 17}, {-610, 416, 20}, {-606, 416, 20}},
// TEX_TIRE_SIDE

{{-610, 420, 16}, {-610, 417, 17}, {-606, 417, 17}},
// TEX_TIRE_SIDE

{{-610, 423, 17}, {-610, 420, 16}, {-606, 420, 16}},
// TEX_TIRE_SIDE

{{-610, 424, 20}, {-610, 423, 17}, {-606, 423, 17}},
// TEX_TIRE_SIDE

{{-606, 395, 17}, {-606, 394, 20}, {-606, 395, 23}},
// TEX_TIRE_FRONT_BACK

{{-610, 398, 24}, {-610, 395, 23}, {-610, 394, 20}},
// TEX_TIRE_FRONT_BACK

{{-610, 401, 23}, {-610, 402, 20}, {-606, 402, 20}},
// TEX_TIRE_SIDE

{{-610, 398, 24}, {-610, 401, 23}, {-606, 401, 23}},
// TEX_TIRE_SIDE

{{-610, 395, 23}, {-610, 398, 24}, {-606, 398, 24}},
// TEX_TIRE_SIDE

{{-610, 394, 20}, {-610, 395, 23}, {-606, 395, 23}},
// TEX_TIRE_SIDE

{{-610, 395, 17}, {-610, 394, 20}, {-606, 394, 20}},
// TEX_TIRE_SIDE

{{-610, 398, 16}, {-610, 395, 17}, {-606, 395, 17}},
// TEX_TIRE_SIDE

{{-610, 401, 17}, {-610, 398, 16}, {-606, 398, 16}},
// TEX_TIRE_SIDE

{{-610, 402, 20}, {-610, 401, 17}, {-606, 401, 17}},
// TEX_TIRE_SIDE

{{-582, 395, 17}, {-582, 394, 20}, {-582, 395, 23}},
// TEX_TIRE_FRONT_BACK

{{-586, 398, 24}, {-586, 395, 23}, {-586, 394, 20}},
// TEX_TIRE_SIDE

{{-586, 401, 23}, {-586, 402, 20}, {-582, 402, 20}},
// TEX_TIRE_SIDE

{{-586, 398, 24}, {-586, 401, 23}, {-582, 401, 23}},
// TEX_TIRE_SIDE

{{-586, 395, 23}, {-586, 398, 24}, {-582, 398, 24}},
// TEX_TIRE_SIDE

{{-586, 394, 20}, {-586, 395, 23}, {-582, 395, 23}},
// TEX_TIRE_SIDE

{{-586, 395, 17}, {-586, 394, 20}, {-582, 394, 20}},
// TEX_TIRE_SIDE

{{-586, 398, 16}, {-586, 395, 17}, {-582, 395, 17}},
// TEX_TIRE_SIDE

{{-586, 401, 17}, {-586, 398, 16}, {-582, 398, 16}},
// TEX_TIRE_SIDE

{{-586, 402, 20}, {-586, 401, 17}, {-582, 401, 17}},
// TEX_TIRE_FRONT_BACK

{{-582, 417, 17}, {-582, 416, 20}, {-582, 417, 23}},
// TEX_TIRE_FRONT_BACK

{{-586, 420, 24}, {-586, 417, 23}, {-586, 416, 20}},
// TEX_TIRE_FRONT_BACK

{{-586, 423, 23}, {-586, 424, 20}, {-582, 424, 20}},
// TEX_TIRE_SIDE

{{-586, 420, 24}, {-586, 423, 23}, {-582, 423, 23}},
// TEX_TIRE_SIDE

{{-586, 417, 23}, {-586, 420, 24}, {-582, 420, 24}},
// TEX_TIRE_SIDE

{{-586, 416, 20}, {-586, 417, 23}, {-582, 417, 23}},
// TEX_TIRE_SIDE

{{-586, 417, 17}, {-586, 416, 20}, {-582, 416, 20}},
// TEX_TIRE_SIDE

{{-586, 420, 16}, {-586, 417, 17}, {-582, 417, 17}},
// TEX_TIRE_SIDE

{{-586, 423, 17}, {-586, 420, 16}, {-582, 420, 16}},
// TEX_TIRE_SIDE

{{-586, 424, 20}, {-586, 423, 17}, {-582, 423, 17}}
// TEX_TIRE_SIDE
};
