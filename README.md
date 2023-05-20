# Smart_Traffic_light
Smart Traffic light [Embedded System Project]

# Introduction

Traffic jam is a problem that is often faced by people especially those living in crowded cities, which is wasting time and money. Mainly, traffic light has been the common cause for traffic jam. Almost all traffic lights that caused traffic jam, work conventionally with timer. This ineffective method sometime needs Police's intervention to reduce traffic jam. Basically, Police rules traffic based on traffic density. Indeed, this paper proposes adaptive traffic lights to control its timing through traffic density calculation on road pattern. The traffic density is obtained by using image processing on several road pattern. Later, the traffic density is utilized to calculate timing of the traffic light. There is a server that collects the data and controls the traffic light operations in a crossroad. The entire algorithms, including the calculation of traffic density and timing of traffic light are validated through real road condition. The results show that traffic density detection system has good accuracy and timing of traffic light is successfully calculated.

## Smart Traffic Technology

Smart traffic technology is a recent development that incorporates AI to manage traffic flow.

These systems use machine learning algorithms to analyze traffic data in real-time and adjust
traffic signals accordingly.

One example of this technology is the Adaptive Traffic Control System (ATCS), which was
developed in the US in the 1990s.

This system uses machine learning algorithms to optimize traffic flow by adjusting signal
timings based on traffic patterns.

Another example of smart traffic technology is the intelligent transportation system (ITS) used
in Singapore.


## Technologies used in smart traffic lights:
1. [ ] Sensors: Smart traffic lights often use sensors to detect the presence of vehicles or pedestrians at an intersection.

     These sensors can be embedded in the road or mounted above it, and they can use a variety of technologies such as radar, infrared, or magnetic fields to          detect vehicles.

2. [ ] Cameras: Some smart traffic lights use cameras to monitor traffic conditions and adjust signal timings accordingly.

     These cameras can be used to count vehicles, detect the presence of pedestrians, or
     even identify specific vehicles through license plate recognition.

3. [ ] Wireless communication: Smart traffic lights often use wireless communication
technologies such as Wi-Fi or cellular networks to communicate with other traffic lights,
central control systems, or connected vehicles.

     This allows them to coordinate with other signals and adjust their timings based on
     real-time traffic conditions.

4. [ ] Machine learning: Some smart traffic lights use machine learning algorithms to analyze
traffic patterns and optimize signal timings.

      These algorithms can learn from historical traffic data and adjust signal timings in realtime to improve traffic flow and reduce congestion.

5. [ ] LED lighting: LED lighting is commonly used in smart traffic lights because it is more
       energy-efficient and longer-lasting than traditional incandescent bulbs.

      LED lights can also be programmed to change color and brightness depending on
      traffic conditions, improving visibility for drivers and pedestrians.

6. [ ] Power management: Smart traffic lights often use power management technologies to
reduce energy consumption and extend the life of their components.

For example, they may use low-power modes when traffic is light or shut down completely during off-peak hours.

# Proposed system 

A smart traffic light control system intended for intelligent allocation of right of way based on vehicle densities sensing, allotting of due passage duration time, and overriding feature for emergency cases, is proposed and implemented in this paper. To realize the three set objectives, three PIC 16F877A microcontrollers, one specifically dedicated for each of the three tasks, are deployed for the logic control design. Three PIC16F877A microcontrollers are used to provide enough number of pins for the three tasks. A Proteus Labcenter circuit design of the system was first simulated as shown in Fig. 1 before actual implementation. Each of the microcontrollers is a CMOS FLASH-based 8-bit microcontroller with RISC architecture. The system contains eight pressure (PR) sensors, two each in each of the four roads. The PR sensors are located at a distance between each other such that when both of them experience continuous depression for specified seconds, an overriding signal is logically sent to the microcontroller. To achieve the control design, the microcontrollers are set into operation by writing sets of instructions (using C language) which are assembled and burnt into the microcontroller chip.

# Phase design

The main aim of the phase design is to differentiate the conflicting movements in an intersection into various phases, so that movements in a phase should have no conflicts. Since all the movements are to be separated with no conflicts, then a large number of phases are required. In such a situation, the main aim is to design phases with minimum or less severe conflicts. There is no precise methodology for the design of phases. This is often guided by the geometry of the intersection, flow pattern especially the turning movements, the relative magnitudes of flow. Therefore, a trial and error procedure is often adopted. However, phase design is very important because it affects the further design steps. For the 4-way road traffic in this design, a model of eight phase signal system is adopted to ensure adequate separation of conflicting movements.The phase designs of the different phases adopted in this paper are illustrated in Figs. 2(a) to 2(h).

Figure 1. A Proteus Labcenter circuit design of the system.
![image](https://github.com/khaled-taha/Smart_Traffic_light/assets/61011535/405f662d-9a8c-446d-a6c5-9d5c92101987)

![image](https://github.com/khaled-taha/Smart_Traffic_light/assets/61011535/5efb1da5-4f25-4746-9e7e-d6500aa139f9)

![image](https://github.com/khaled-taha/Smart_Traffic_light/assets/61011535/e1945d9b-cc90-4e72-ba3b-b782c3aeeaec)

![image](https://github.com/khaled-taha/Smart_Traffic_light/assets/61011535/d763dc07-d051-4896-b303-063d3c59b32d)

![image](https://github.com/khaled-taha/Smart_Traffic_light/assets/61011535/ca7144f5-a58e-44cd-b16a-b071a4dcd773)



# The Future of Smart Traffic Technology:

## Challenges and Opportunities:
Another survey on the future of smart traffic technology highlights the challenges and
opportunities that lie ahead. This survey discusses the potential benefits of smart traffic
technology, including reduced congestion and emissions, improved safety, and enhanced
mobility.

