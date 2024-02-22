### 1. current progress

- [x] basic shapes

- [x] CSG operations : except for shell and blend

- [ ] humanoid

  



![image-20240208104008159](presentationImg/image-20240208104008159.png)

### CSG 

- UNION as example
- ![image-20240208111222978](presentationImg/image-20240208111222978.png)

### Raymarcher

- input : camera, resolution, minimum transmissity , $\Delta s$,  $\kappa$,  

- RayMarcher::RayMarch

  - input: density field, color field

- RayMarcher::MarchSigleLight

  - input: ray index, density field, color field

  - near and far plane; if snear and sfar are constants for each ray, the near plane and the far plane will form an arc, not a plane

    <img src="C:\Users\xinhu\AppData\Roaming\Typora\typora-user-images\image-20240208112917938.png" alt="image-20240208112917938"  />

  - <img src="presentationImg\image-20240208114956597.png" alt="image-20240208114956597" style="zoom: 33%;" />

  - <img src="presentationImg\image-20240208115216501.png" alt="image-20240208115216501" style="zoom:33%;" />

  

## HUMANOID

- Skirt as example

  

### Performance

- Rotation: use Transpose instead of Inverse. time consumption decreased from 16s to 12s

   no more need to calculate cofactor matrix for each element: $O(N^2)$ to $O(N)$

- pass smart pointer by reference for all CSG operation: 12s -> 9s  

### Demo



![image-20240208112634851](presentationImg/image-20240208112634851.png)



### Unsolved Issues

- the order of CSG operation affects efficiency

  - Skirt for example,

     if I assign a color field to the sdf and then rotate the colorfield while rotating sdf . 

    time consumption from 9->8

- for some CSG operation:

  take union as example:

  passing shared_ptr by value makes app run faster than passing by reference: 

  9s -> 8s