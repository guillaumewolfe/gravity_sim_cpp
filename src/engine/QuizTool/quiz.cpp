// Quiz.cpp
#include "engine/QuizTools/quiz.h"
#include <iostream>
#include <algorithm>
#include <numeric>

// Constructeur
Quiz::Quiz(){

}

//destructeur
Quiz::~Quiz(){

}

void Quiz::demarrer(){
    score = 0;

    //On mélange les questions:
    std::random_shuffle(questions.begin(), questions.end());

    //On mélange les réponses dans les questions:
    for (auto& question : questions){
        question.shuffle();
    }
}

void Quiz::addQuestion(Question question){
    questions.push_back(question);
}

float Quiz::getScore() const{
    //return score/nbre de question
    return score/numberOfQuestionsPosed;
}


void Quiz::generateEasyQuiz() {
    addQuestion(Question("What is the largest planet in our solar system?", {"Jupiter", "Mars", "Earth", "Venus"}, 0, "Jupiter is the largest planet in our solar system, with a mass more than twice that of all the other planets combined. Its Great Red Spot, a giant storm, is larger than Earth."));
    addQuestion(Question("What planet is known as the Red Planet?", {"Mars", "Jupiter", "Saturn", "Venus"}, 0, "Mars is known as the Red Planet due to its reddish appearance, caused by iron oxide (rust) on its surface."));
    addQuestion(Question("How many planets are in our solar system?", {"8", "9", "7", "10"}, 0, "There are 8 planets in our solar system: Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, and Neptune."));
    addQuestion(Question("What does the Earth orbit?", {"The Sun", "The Moon", "Mars", "Jupiter"}, 0, "The Earth orbits the Sun, which is the center of our solar system."));
    addQuestion(Question("Which planet is known for its rings?", {"Saturn", "Jupiter", "Mars", "Earth"}, 0, "Saturn is known for its beautiful rings, which are made of ice and rock particles. "));
    addQuestion(Question("What is the smallest planet in our solar system?", {"Mercury", "Mars", "Earth", "Jupiter"}, 0, "Mercury is the smallest planet in our solar system, and it is also the closest to the Sun."));
    addQuestion(Question("What is the name of our galaxy?", {"The Milky Way", "Andromeda", "Sombrero", "Whirlpool"}, 0, "Our galaxy is called the Milky Way, and it contains billions of stars, including our Sun."));
    addQuestion(Question("Which planet is known as the Earth's twin?", {"Venus", "Mars", "Jupiter", "Saturn"}, 0, "Venus is often called Earth's twin because it is similar in size and composition, but it has a thick, toxic atmosphere."));
    addQuestion(Question("What is the main component of the Sun?", {"Hydrogen", "Oxygen", "Carbon", "Iron"}, 0, "The Sun is primarily composed of hydrogen, which fuels its nuclear fusion reactions."));
    addQuestion(Question("What unit is used to measure distances in space?", {"Light-year", "Kilometer", "Mile", "Femtometer"}, 0, "A light-year is the distance that light travels in one year, and it is often used to measure vast distances in space."));
    addQuestion(Question("Which force keeps the planets in orbit around the Sun?", {"Gravity", "Magnetic", "Electrical", "Nuclear"}, 0, "Gravity is the force that keeps the planets in orbit around the Sun, and it also holds galaxies together."));
    addQuestion(Question("What type of star is the Sun?", {"Yellow dwarf", "Red giant", "White dwarf", "Neutron star"}, 0, "The Sun is classified as a yellow dwarf star, and it is in the middle of its life cycle."));
    addQuestion(Question("What is a light-year?", {"A unit of time", "A unit of speed", "A unit of distance", "A unit of light intensity"}, 2,  "A light-year is a unit of distance, equal to the distance that light travels in one year."));
    addQuestion(Question("Which planet has the strongest gravitational pull?", {"Jupiter", "Earth", "Neptune", "Mars"}, 0, "Jupiter has the strongest gravitational pull of any planet in our solar system, due to its large mass."));
    addQuestion(Question("What phenomenon allows black holes to be detected?", {"X-ray emissions", "Visible light", "Radio waves", "Ultraviolet radiation"}, 0, "Black holes can be detected by the X-ray emissions from the hot gas swirling around them."));
    addQuestion(Question("What force is responsible for the fusion reactions in the Sun?", {"Gravitational", "Electromagnetic", "Strong nuclear", "Weak nuclear"}, 2, "The strong nuclear force is responsible for the fusion reactions in the Sun, which convert hydrogen into helium."));
    addQuestion(Question("What is the main sequence?", {"A type of galaxy", "A phase in a star's life", "A model of the universe", "A classification of asteroids"}, 1, "The main sequence is a phase in a star's life cycle, where it fuses hydrogen into helium in its core."));
    addQuestion(Question("What do astronomers use to measure the chemical composition of stars?", {"Telescopes", "Spectroscopes", "Barometers", "Thermometers"}, 1, "Astronomers use spectroscopes to measure the chemical composition of stars by analyzing the light they emit."));
    addQuestion(Question("What is the primary source of the Sun's energy?", {"Nuclear fusion", "Nuclear fission", "Chemical reactions", "Gravitational collapse"}, 0, "The Sun's energy comes from nuclear fusion reactions in its core, where hydrogen is converted into helium."));
    addQuestion(Question("Which planet is known for having a day longer than its year?", {"Venus", "Jupiter", "Mars", "Mercury"}, 0, "Venus has a day that is longer than its year, due to its slow rotation on its axis."));
    addQuestion(Question("What causes the phases of the Moon?", {"The Earth's shadow", "The Sun's position", "The Moon's orbit around the Earth", "The Moon's rotation"}, 2, "The phases of the Moon are caused by the Moon's orbit around the Earth, which changes the angle of sunlight hitting its surface."));
    addQuestion(Question("What is an astronomical unit (AU)?", {"The distance from the Earth to the Moon", "The distance from the Earth to the Sun", "The average radius of the Earth", "The distance across the Milky Way"}, 1, "An astronomical unit (AU) is the average distance from the Earth to the Sun, we use it to measure distances within our solar system."));
    addQuestion(Question("What is the term for when the Moon completely covers the Sun?", {"Solar eclipse", "Lunar eclipse", "Total eclipse", "Annular eclipse"}, 0, "A solar eclipse occurs when the Moon passes between the Earth and the Sun, blocking the Sun's light."));
    addQuestion(Question("Which celestial body has the highest density?", {"Earth", "Sun", "Jupiter", "Mercury"}, 0, "Earth has the highest density of any planet in our solar system, due to its solid, rocky composition."));
    addQuestion(Question("What type of galaxy is the Milky Way?", {"Spiral", "Elliptical", "Irregular", "Lenticular"}, 0, "The Milky Way is a spiral galaxy, with a central bulge and spiral arms of stars, gas, and dust."));
    addQuestion(Question("Which layer of the Sun do we see from Earth?", {"Core", "Radiative Zone", "Convective Zone", "Photosphere"}, 3, "The photosphere is the visible surface of the Sun, where most of its light and heat are emitted."));
    addQuestion(Question("What is the name of the effect caused by Earth's rotation that affects weather patterns?", {"Coriolis effect", "Greenhouse effect", "Doppler effect", "Albedo effect"}, 0, "The Coriolis effect is caused by the Earth's rotation, and it affects the direction of winds and ocean currents."));
    addQuestion(Question("Which of the following is a dwarf planet?", {"Europa", "Pluto", "Venus", "Mars"}, 1, "Pluto is classified as a dwarf planet, and it was reclassified from a full planet in 2006."));
    addQuestion(Question("What is the nearest planet to the Sun?", {"Mercury", "Venus", "Earth", "Mars"}, 0, "Mercury is the closest planet to the Sun, and it has a rocky, cratered surface."));
    addQuestion(Question("What galaxy is Earth located in?", {"The Milky Way", "Andromeda", "The Whirlpool", "The Sombrero"}, 0, "Earth is located in the Milky Way galaxy, which is a spiral galaxy containing billions of stars."));
    addQuestion(Question("What is the main component of the Sun's gas?", {"Hydrogen", "Helium", "Oxygen", "Carbon"}, 0, "The Sun is primarily composed of hydrogen gas, which fuels its nuclear fusion reactions."));
    addQuestion(Question("Which planet is known as the Earth's Twin?", {"Venus", "Mars", "Jupiter", "Mercury"}, 0, "Venus is often called Earth's twin because it is similar in size and composition, but it has a thick, toxic atmosphere."));
    addQuestion(Question("How long does it take for the Earth to orbit the Sun?", {"365 days", "24 hours", "30 days", "10 years"}, 0, "It takes the Earth approximately 365 days, or one year, to orbit the Sun."));
    addQuestion(Question("What is the primary component of a comet's tail?", {"Ice and dust", "Rock", "Lava", "Metal"}, 0, "A comet's tail is primarily made of ice and dust particles that are illuminated by the Sun as the comet approaches it."));
    addQuestion(Question("Which celestial event is caused by the Earth passing through the debris left by a comet?", {"Meteor shower", "Solar eclipse", "Lunar eclipse", "Aurora"}, 0, "Meteor showers occur when Earth passes through the debris left by a comet, causing the debris to burn up in Earth's atmosphere."));
    addQuestion(Question("What is the lightest element that fuels the fusion reactions in the Sun?", {"Hydrogen", "Helium", "Carbon", "Oxygen"}, 0, "Hydrogen is the lightest and most abundant element in the universe, fueling the fusion reactions in the Sun."));
    addQuestion(Question("How fast does light travel in a vacuum?", {"299,792 kilometers per second", "150,000 kilometers per second", "1,000,000 kilometers per second", "30,000 kilometers per second"}, 0, "Light travels at a speed of 299,792 kilometers per second in a vacuum, which is about 186,282 miles per second."));
    addQuestion(Question("What is the phenomenon where light bends around a massive object?", {"Gravitational lensing", "Refraction", "Diffraction", "Reflection"}, 0, "Gravitational lensing is a phenomenon where light bends around a massive object, like a galaxy or a black hole, due to gravity."));
    addQuestion(Question("Which planet is known as the 'Morning Star' or the 'Evening Star'?", {"Venus", "Mars", "Jupiter", "Mercury"}, 0, "Venus is known as the 'Morning Star' or the 'Evening Star' due to its bright appearance in the sky at dawn and dusk."));
    addQuestion(Question("What type of galaxy is the Milky Way?", {"Spiral", "Elliptical", "Irregular", "Lenticular"}, 0, "The Milky Way is a spiral galaxy, characterized by its flat, disk-like shape with a bulging center and spiral arms."));
    addQuestion(Question("What is the term for the explosion of a star at the end of its life?", {"Supernova", "Nova", "Red giant collapse", "Black hole formation"}, 0, "A supernova is the explosion of a star at the end of its life, resulting in a massive release of energy."));
    addQuestion(Question("What is the term used to describe the boundary around a black hole from which nothing can escape?", {"Event horizon", "Schwarzschild radius", "Accretion disk", "Singularity"}, 0, "The event horizon is the boundary around a black hole from which nothing, not even light, can escape."));
    addQuestion(Question("How much does a teaspoon of neutron star material weigh?", {"As much as a mountain", "10 million tons", "The same as a car", "About 6 billion tons"}, 1, "A teaspoon of neutron star material weighs about 10 million tons, due to its incredibly dense composition."));
    addQuestion(Question("Approximately how long does it take for light from the Sun to reach Earth?", {"8 seconds", "1 second", "8 minutes", "20 minutes"}, 2, "We're seeing the Sun from 'the past'; it takes light approximately 8 minutes to travel from the Sun to Earth."));
}

void Quiz::generateMediumQuiz() {
    addQuestion(Question("Which planet has the most moons?", {"Jupiter", "Saturn", "Mars", "Earth"}, 0, "Jupiter has the most moons of any planet in our solar system, with over 70 known moons. The second most is Saturn, with over 60 moons."));
    addQuestion(Question("What is the name of Mars' largest moon?", {"Phobos", "Deimos", "Ganymede", "Titan"}, 0, "Mars' largest moon is named Phobos, and it is one of the smallest moons in the solar system."));
    addQuestion(Question("What is the temperature on the surface of Venus?", {"462°C", "100°C", "-5°C", "15°C"}, 0, "The surface of Venus is extremely hot, with temperatures reaching up to 462°C due to its thick atmosphere and greenhouse effect."));
    addQuestion(Question("What is the Great Red Spot on Jupiter?", {"A storm", "A mountain", "A desert", "A lake"}, 0, "The Great Red Spot is a giant storm on Jupiter, which has been raging for at least 400 years."));
    addQuestion(Question("Which planet has the longest day?", {"Venus", "Earth", "Jupiter", "Mars"}, 0, "Venus has the longest day of any planet in our solar system, with a day lasting longer than its year."));
    addQuestion(Question("What is the Kuiper Belt?", {"A region of icy bodies", "A single asteroid", "A star system", "A galaxy"}, 0, "The Kuiper Belt is a region of icy bodies beyond Neptune, including dwarf  planets like Pluto and Eris."));
    addQuestion(Question("What causes a solar eclipse?", {"The Moon passing between the Sun and Earth", "Earth passing between the Sun and the Moon", "Jupiter passing between the Sun and Earth", "The Sun moving behind the Moon"}, 0, "A solar eclipse occurs when the Moon passes between the Sun and Earth, blocking the Sun's light."));
    addQuestion(Question("What is the brightest planet in the night sky?", {"Venus", "Mars", "Jupiter", "Saturn"}, 0, "Venus is the brightest planet in the night sky, and it is often called the 'evening star' or 'morning star.'"));
    addQuestion(Question("What is the dwarf planet located in the asteroid belt?", {"Ceres", "Pluto", "Eris", "Makemake"}, 0, "Ceres is the largest object in the asteroid belt between Mars and Jupiter, and it is classified as a dwarf planet."));
    addQuestion(Question("What phenomenon causes the Northern and Southern Lights?", {"Solar wind", "Lunar reflection", "Earth's magnetic field", "Sunspots"}, 0, "The Northern and Southern Lights are caused by solar wind particles interacting with the Earth's magnetic field."));
    addQuestion(Question("What is the term for the point in a planet's orbit closest to the Sun?", {"Perihelion", "Aphelion", "Perigee", "Apogee"}, 0, "Perihelion is the point in a planet's orbit where it is closest to the Sun, while aphelion is the point farthest from the Sun."));
    addQuestion(Question("What is the event horizon of a black hole?", {"The point of no return", "The outermost layer", "The center", "A hypothetical boundary"}, 0, "The event horizon of a black hole is the point of no return, beyond which nothing, not even light, can escape."));
    addQuestion(Question("Which type of star is most common in the Milky Way?", {"Red dwarf", "Blue giant", "Supergiant", "White dwarf"}, 0, "Red dwarf stars are the most common type of star in the Milky Way, making up about 70% of all stars."));
    addQuestion(Question("What is the Doppler effect used to measure in astronomy?", {"Distance of stars", "Size of planets", "Velocity of objects moving towards or away from us", "Temperature of celestial bodies"}, 2, "The Doppler effect is used to measure the velocity of objects moving towards or away from us, such as stars and galaxies."));
    addQuestion(Question("What is a neutron star?", {"A collapsing black hole", "A very young star", "A very old star", "The remnant of a supernova"}, 3, "It is incredibly dense, with the mass of the Sun packed into a sphere only a few kilometers across."));
    addQuestion(Question("What causes tides on Earth?", {"The Moon's gravity", "The Sun's gravity", "Earth's rotation", "Jupiter's gravity"}, 0, "Tides on Earth are caused by the gravitational pull of the Moon, which creates bulges in the Earth's oceans."));
    addQuestion(Question("What is a parsec?", {"A unit of time", "A unit of distance", "A unit of speed", "A unit of mass"}, 1, "A parsec is a unit of distance used in astronomy, equal to about 3.26 light-years."));
    addQuestion(Question("What phenomenon explains why the universe is expanding?", {"Relativity", "General Relativity", "Dark Matter", "Quantum Mechanics"}, 1, "The expansion of the universe is explained by the General Relativity, a theory proposed by Albert Einstein that describes the force of gravity and the curvature of spacetime."));
    addQuestion(Question("What kind of object is a quasar?", {"A dying star", "A galaxy", "A rapidly spinning neutron star", "A supermassive black hole at the center of a galaxy"}, 3, "A quasar is a supermassive black hole at the center of a galaxy, which emits intense radiation as it consumes surrounding material."));
    addQuestion(Question("What is Hawking radiation?", {"Radiation emitted by black holes", "Radiation from the Sun", "Radiation from nuclear fusion", "Background cosmic radiation"}, 0, "Hawking radiation is a theoretical form of radiation emitted by black holes, proposed by physicist Stephen Hawking."));
    addQuestion(Question("What does the term 'exoplanet' refer to?", {"Planets outside the Solar System", "Moons of Jupiter", "Asteroids in the Kuiper belt", "Comets passing near Earth"}, 0, "An exoplanet is a planet that orbits a star outside of our Solar System, and thousands have been discovered."));
    addQuestion(Question("What is the primary factor that determines a star's color?", {"Its distance from Earth", "Its temperature", "Its size", "Its age"}, 1, "A star's color is primarily determined by its temperature, with hotter stars appearing blue and cooler stars appearing red."));
    addQuestion(Question("What is the Schwarzschild radius?", {"The core of a galaxy", "The event horizon of a black hole", "The distance at which a star can form", "The radius of a neutron star"}, 1, "The Schwarzschild radius is the radius of the event horizon of a black hole, beyond which nothing can escape."));
    addQuestion(Question("Which force is primarily responsible for the formation of stars?", {"Electromagnetic force", "Strong nuclear force", "Gravity", "Weak nuclear force"}, 2, "Gravity is the primary force responsible for the formation of stars, as it causes gas and dust to collapse and form dense cores."));
    addQuestion(Question("What are pulsars?", {"Collapsing stars", "Rapidly spinning neutron stars", "Black holes emitting X-rays", "Distant galaxies"}, 1, "Pulsars are rapidly spinning neutron stars that emit beams of radiation, which appear as regular pulses as they rotate."));
    addQuestion(Question("What does 'redshift' indicate about a galaxy's motion?", {"It is moving towards us", "It is stationary", "It is moving away from us", "It is spinning"}, 2, "Redshift indicates that a galaxy is moving away from us, as the light waves are stretched to longer, redder wavelengths."));
    addQuestion(Question("What is the James Webb Space Telescope primarily designed to observe?", {"The early universe", "The Sun's corona", "Nearby asteroids", "The outer planets"}, 0, "The James Webb Space Telescope is designed to observe the early universe, including the first galaxies and stars."));
    addQuestion(Question("What element is the endpoint of fusion in the core of a massive star?", {"Iron", "Hydrogen", "Carbon", "Helium"}, 0, "Iron cannot be fused into heavier elements without requiring more energy than it releases."));
    addQuestion(Question("What is the Kuiper Belt?", {"A region of icy bodies beyond Neptune", "A belt of asteroids between Mars and Jupiter", "A distant ring of black holes", "A galaxy cluster"}, 0, "The Kuiper Belt is a region of icy bodies beyond Neptune, including dwarf planets like Pluto and Eris."));
    addQuestion(Question("What phenomenon causes a solar eclipse?", {"The Moon passing between the Sun and Earth", "The Earth passing between the Sun and Moon", "The Sun passing between the Earth and Moon", "Mars passing between the Earth and Sun"}, 0, "A solar eclipse occurs when the Moon passes between the Sun and Earth, blocking the Sun's light."));
    addQuestion(Question("Which planet has a day longer than its year?", {"Venus", "Mercury", "Jupiter", "Saturn"}, 0, "Venus has a day longer than its year, due to its slow rotation on its axis."));
    addQuestion(Question("What are Saturn's rings made of?", {"Ice and rock", "Gas", "Liquid water", "Dust"}, 0, "Saturn's rings are made of ice and rock particles, ranging in size from tiny grains to large boulders."));
    addQuestion(Question("What is the name of the effect that causes the Earth's rotation to affect weather patterns?", {"Coriolis effect", "Greenhouse effect", "Doppler effect", "Magnetic effect"}, 0, "The Coriolis effect is caused by the Earth's rotation, and it affects the direction of winds and ocean currents."));
    addQuestion(Question("How fast does light travel in a vacuum?", {"299,792 km/s", "150,000 km/s", "1,000,000 km/s", "500,000 km/s"}, 0, "Light travels at a speed of 299,792 kilometers per second in a vacuum, which is considered the universal speed limit."));
    addQuestion(Question("What is the term for a moon that orbits another moon?", {"Submoon", "Moonlet", "Mini-moon", "Satellite moon"}, 0, "A 'submoon' is a theoretical term for a moon that orbits another moon, though none have been observed yet."));
    addQuestion(Question("What causes the seasons on Earth?", {"Earth's distance from the Sun", "The Sun's magnetic field", "Earth's axial tilt", "The Moon's orbit"}, 2, "The seasons are caused by the axial tilt of the Earth, not its distance from the Sun. This tilt results in varying amounts of sunlight hitting different parts of Earth during its orbit."));
    addQuestion(Question("What is the Oort Cloud?", {"A cloud of gas and dust between Mars and Jupiter", "A region of icy objects surrounding our solar system", "A nebula within the Milky Way", "A cluster of galaxies near the Milky Way"}, 1, "The Oort Cloud is a theoretical cloud of predominantly icy objects that is believed to surround the solar system, far beyond Pluto."));
    addQuestion(Question("Which gas is most abundant in the atmosphere of Venus?", {"Carbon Dioxide", "Nitrogen", "Oxygen", "Hydrogen"}, 0, "Carbon Dioxide is the most abundant gas in Venus's atmosphere, contributing to its extreme greenhouse effect."));
    addQuestion(Question("What is the largest type of star in the universe?", {"Red Giant", "Supergiant", "White Dwarf", "Neutron Star"}, 1, "Supergiant stars are the largest type of stars in the universe, often having diameters hundreds of times larger than the Sun."));
    addQuestion(Question("What is the main component of a comet's tail?", {"Rock", "Ice and dust", "Hydrogen gas", "Iron particles"}, 1, "A comet's tail is primarily made of ice and dust that has been vaporized, creating a glowing trail."));
    addQuestion(Question("Which planet has the most extreme temperature fluctuations?", {"Mercury", "Venus", "Earth", "Mars"}, 0, "Mercury has the most extreme temperature fluctuations.With no atmosphere to retain heat, temperatures can swing over 600 degrees Celsius."));
    addQuestion(Question("What is the cosmic microwave background radiation?", {"Heat from the Sun", "Radiation from Earth's core", "A glow from the Big Bang", "Light from distant galaxies"}, 2, "The cosmic microwave background radiation is a faint glow left over from the Big Bang, pervading the entire universe."));
    addQuestion(Question("What are the Van Allen Belts?", {"Rings of Saturn", "A type of asteroid", "Layers of the Sun's atmosphere", "Radiation belts around Earth"}, 3, "The Van Allen Belts are two radiation belts around Earth, trapped by the planet's magnetic field, and pose a challenge for space travel."));
}

void Quiz::generateHardQuiz() {
    addQuestion(Question("Which spacecraft made the first successful flyby of Mars?", {"Mariner 4", "Voyager 1", "New Horizons", "Pioneer 10"}, 0, "Mariner 4 was the first spacecraft to successfully fly by Mars, capturing the first close-up images of the planet in 1965."));
    addQuestion(Question("What is the Oort Cloud theorized to be?", {"A distant spherical shell of icy objects", "A nearby asteroid belt", "A galaxy cluster", "A nebula surrounding our solar system"}, 0, "The Oort Cloud is theorized to be a distant spherical shell of icy objects, located far beyond the orbit of Pluto."));
    addQuestion(Question("What is the largest moon of Saturn?", {"Titan", "Europa", "Ganymede", "Callisto"}, 0, "Titan is the largest moon of Saturn, and it is the second largest moon in the solar system, after Jupiter's Ganymede."));
    addQuestion(Question("What is the theory that describes the formation of the solar system?", {"Nebular hypothesis", "Big Bang theory", "Steady State theory", "Panspermia theory"}, 0, "The nebular hypothesis is the theory that describes the formation of the solar system from a rotating cloud of gas and dust."));
    addQuestion(Question("What is the Perseverance rover's primary mission on Mars?", {"Search for signs of ancient life", "Map the Martian surface", "Study Martian weather", "Prepare for human colonization"}, 0, "The Perseverance rover's primary mission is to search for signs of ancient life on Mars, as well as collect samples for future return to Earth."));
    addQuestion(Question("What is the name of the boundary marking the end of the solar system?", {"The heliopause", "The Kuiper belt", "The Oort cloud", "The asteroid belt"}, 0, "The heliopause is the boundary marking the end of the solar system, where the solar wind meets the interstellar medium."));
    addQuestion(Question("Which planet has a day longer than its year?", {"Venus", "Jupiter", "Mercury", "Mars"}, 0, "Venus has a day longer than its year, due to its slow rotation on its axis."));
    addQuestion(Question("What are the dark, flat areas on the Moon called?", {"Mare", "Craters", "Highlands", "Rilles"}, 0, "The dark, flat areas on the Moon are called mare, which is Latin for 'sea,' and they are ancient volcanic plains."));
    addQuestion(Question("What is the main difference between a comet and an asteroid?", {"Comets are icy, asteroids are rocky", "Asteroids are larger than comets", "Comets can support life, asteroids cannot", "Asteroids orbit the Sun, comets do not"}, 0, "The main difference between a comet and an asteroid is that comets are icy bodies, while asteroids are rocky bodies."));
    addQuestion(Question("Which of these objects is classified as a dwarf planet?", {"Eris", "Europa", "Enceladus", "Edgeworth"}, 0, "Eris is classified as a dwarf planet, and it is one of the largest known objects in the Kuiper Belt."));
    addQuestion(Question("What is the Chandrasekhar limit?", {"The maximum mass of a white dwarf", "The density of a neutron star", "The size of a black hole", "The luminosity of a supernova"}, 0, "The Chandrasekhar limit is the maximum mass of a white dwarf star, beyond which it will collapse and explode as a supernova."));
    addQuestion(Question("What does the Hertzsprung-Russell diagram display?", {"The age of stars", "The distance of stars from Earth", "The relationship between star brightness and temperature", "The speed of stars"}, 2, "The Hertzsprung-Russell diagram displays the relationship between a star's brightness and temperature, and it is used to classify stars and understand their life cycles."));
    addQuestion(Question("What is redshift and what does it indicate about celestial objects?", {"A shift towards the red end of the spectrum, indicating they are moving away from us", "A shift towards the blue end of the spectrum, indicating they are moving towards us", "A change in the star's color due to temperature", "A measurement of the star's chemical composition"}, 0, "Redshift is a shift towards the red end of the spectrum, indicating that celestial objects are moving away from us, while blueshift indicates they are moving towards us."));
    addQuestion(Question("What is the cosmological principle?", {"The universe is homogeneous and isotropic", "The universe has an edge", "The universe is centered around the Earth", "The universe is infinite"}, 0, "The cosmological principle states that the universe is homogeneous and isotropic, meaning it looks the same in all directions and at all locations."));
    addQuestion(Question("What concept explains the acceleration of the universe's expansion?", {"Dark energy", "Dark matter", "Antimatter", "Neutrinos"}, 0, "Dark energy is the concept that explains the acceleration of the universe's expansion, and it is thought to make up about 68% of the universe's total energy."));
    addQuestion(Question("What are Magellanic Clouds?", {"Nearby galaxies", "Types of nebulae", "Black holes", "Clusters of stars in the Milky Way"}, 0, "The Magellanic Clouds are two nearby irregular galaxies that are companions to the Milky Way, and they are visible from the southern hemisphere."));
    addQuestion(Question("What is the significance of the cosmic microwave background radiation?", {"Evidence of the Big Bang", "Radiation from the Sun", "Background noise from Earth's atmosphere", "Light from distant galaxies"}, 0, "The cosmic microwave background radiation is considered evidence of the Big Bang, as it is the afterglow of the early universe, and it is nearly uniform in all directions."));
    addQuestion(Question("What principle is used to measure the mass of galaxies?", {"Kepler's laws", "Newton's law of gravitation", "Einstein's theory of relativity", "Hubble's law"}, 1, "Newton's law of gravitation is used to measure the mass of galaxies, by observing the motion of stars and gas within them."));
    addQuestion(Question("What are Type Ia supernovae used for in astronomy?", {"Measuring the age of the universe", "Determining distances in the universe", "Studying black holes", "Understanding solar flares"}, 1, "Type Ia supernovae are used to determine distances in the universe, as they are standard candles with a known luminosity."));
    addQuestion(Question("What is the principle behind the operation of a rocket in space?", {"Magneto-hydrodynamics", "Newton's third law of motion", "General relativity", "Quantum mechanics"}, 1, "The operation of a rocket in space is based on Newton's third law of motion, which states that for every action, there is an equal and opposite reaction."));
    addQuestion(Question("What does the Drake equation estimate?", {"The age of the universe", "The size of the observable universe", "The number of communicative civilizations in the Milky Way", "The rate of star formation"}, 2, "The Drake equation estimates the number of communicative civilizations in the Milky Way galaxy, based on factors such as the rate of star formation and the fraction of stars with planets."));
    addQuestion(Question("What is the Alcubierre drive?", {"A type of black hole", "A hypothetical means of faster-than-light travel", "A method for detecting exoplanets", "A design for a new telescope"}, 1, "The Alcubierre drive is a hypothetical means of faster-than-light travel, proposed by physicist Miguel Alcubierre, which involves warping spacetime."));
    addQuestion(Question("What is the Kardashev Scale used to measure?", {"The size of celestial objects", "The energy consumption of a civilization", "The brightness of stars", "The mass of black holes"}, 1, "The Kardashev Scale is used to measure the energy consumption of a civilization, based on its ability to harness and use energy."));
    addQuestion(Question("What phenomenon leads to the spaghettification effect near black holes?", {"Gravitational lensing", "Gravitational redshift", "Tidal forces", "Hawking radiation"}, 2, "Tidal forces near black holes can cause the spaghettification effect, where the gravitational pull on an object becomes stronger than the object's own self-gravity."));
    addQuestion(Question("What does the term 'dark energy' refer to?", {"A hypothetical type of energy that permeates all of space", "The absence of light in outer space", "The energy of black holes", "The energy emitted by dark matter"}, 0, "Dark energy is a hypothetical type of energy that permeates all of space, and it is thought to be responsible for the acceleration of the universe's expansion."));
    addQuestion(Question("What are gamma-ray bursts?", {"Explosions in the Sun's atmosphere", "The birth of new stars", "The most energetic explosions in the universe", "Signals from alien civilizations"}, 2, "Gamma-ray bursts are the most energetic explosions in the universe, thought to be caused by the collapse of massive stars or the merger of neutron stars."));
    addQuestion(Question("What is the significance of the 'Goldilocks Zone'?", {"It is the region around a black hole", "It is the optimal distance from a star where liquid water can exist", "It is the central region of a galaxy", "It is an area in space with a high concentration of gold"}, 1, "The 'Goldilocks Zone' is the optimal distance from a star where liquid water can exist on a planet's surface, making it potentially habitable for life."));
    addQuestion(Question("What principle explains the bending of light around massive objects?", {"Einstein's theory of general relativity", "Newton's law of universal gravitation", "The Doppler effect", "Hubble's law of cosmic expansion"}, 0, "Einstein's theory of general relativity explains the bending of light around massive objects, known as gravitational lensing."));
    addQuestion(Question("What is a quasar?", {"A distant and highly energetic galaxy nucleus", "A type of neutron star", "A small galaxy", "A region in a black hole"}, 0, "A quasar is a distant and highly energetic galaxy nucleus, powered by a supermassive black hole at its center."));
    addQuestion(Question("What is the term for the boundary around a black hole from which no light can escape?", {"Event Horizon", "Photon Sphere", "Singularity", "Accretion Disk"}, 0, "The event horizon is the boundary around a black hole from which no light can escape, marking the point of no return."));
    addQuestion(Question("Who proposed the theory of general relativity, which is crucial for modern cosmology?", {"Albert Einstein", "Isaac Newton", "Stephen Hawking", "Galileo Galilei"}, 0, "Albert Einstein proposed the theory of general relativity, which describes the force of gravity and the curvature of spacetime, and it is crucial for modern cosmology."));
    addQuestion(Question("What is dark matter theorized to be?", {"An undetectable form of mass", "A type of black hole", "A form of energy", "Visible matter"}, 0, "Dark matter is theorized to be an undetectable form of mass, which makes up about 27% of the universe's total energy."));
    addQuestion(Question("Which galaxy is on a collision course with the Milky Way?", {"Andromeda", "Triangulum", "Whirlpool", "Sombrero"}, 0, "The Andromeda galaxy is on a collision course with the Milky Way, and the two galaxies are expected to merge in about 4 billion years."));
    addQuestion(Question("Which element undergoes fusion in the Sun's core to produce energy?", {"Hydrogen", "Helium", "Carbon", "Iron"}, 0, "Hydrogen undergoes fusion to form helium, releasing energy that powers the Sun."));
    addQuestion(Question("What is the Schwarzschild radius?", {"The core of a galaxy", "The event horizon of a black hole", "The diameter of a neutron star", "The distance at which light can escape a black hole"}, 1, "The Schwarzschild radius defines the event horizon of a black hole, beyond which nothing can escape."));
    addQuestion(Question("What does the Drake Equation estimate?", {"The number of active, communicative extraterrestrial civilizations", "The total number of stars in the Milky Way", "The age of the universe", "The likelihood of Earth-like planets"}, 0, "The Drake Equation estimates the number of active, communicative extraterrestrial civilizations in the Milky Way."));
    addQuestion(Question("What phenomenon confirms the theory of General Relativity?", {"Gravitational lensing", "Solar flares", "The Doppler effect", "Quantum entanglement"}, 0, "Gravitational lensing, the bending of light around massive objects, confirms Einstein's General Relativity."));
    addQuestion(Question("What is the significance of the James Webb Space Telescope?", {"Exploring the outer solar system", "Studying Earth's atmosphere", "Observing the early universe", "Mapping the Milky Way"}, 2, "The James Webb Space Telescope is designed to observe the early universe, capturing light from the first galaxies."));
    addQuestion(Question("What are Fast Radio Bursts (FRBs)?", {"Signals from alien civilizations", "Interstellar communication signals", "High-energy astrophysical phenomena", "Solar emissions"}, 2, "FRBs are mysterious high-energy astrophysical phenomena, originating outside the Milky Way."));
    addQuestion(Question("What is the primary evidence for dark matter?", {"Its emission of light", "The rotation curves of galaxies", "Direct detection experiments", "Gravitational waves"}, 1, "The rotation curves of galaxies, which do not match the visible matter alone, suggest the presence of dark matter."));
    addQuestion(Question("What is a pulsar?", {"A rapidly rotating neutron star", "A forming protostar", "A dying red giant", "A colliding galaxy"}, 0, "A pulsar is a rapidly rotating neutron star that emits beams of electromagnetic radiation."));
    addQuestion(Question("What concept does the Fermi Paradox explore?", {"The age of Earth", "The expansion of the universe", "The likelihood of extraterrestrial life", "The origin of the cosmic microwave background"}, 2, "The Fermi Paradox questions why we have not yet detected signs of extraterrestrial life despite the high probability of its existence."));
    addQuestion(Question("What is the Kardashev Scale?", {"A method for detecting exoplanets", "A system for classifying the energy usage of civilizations", "A theory about the structure of black holes", "A measure of the universe's expansion rate"}, 1, "The Kardashev Scale classifies civilizations based on their energy consumption levels."));
    addQuestion(Question("Why does Earth have a higher density than the Sun?",
                        {"Because Earth's composition includes heavy elements such as iron and nickel, which are denser than the Sun's hydrogen and helium","The intense pressure at Earth's core significantly increases its density, a condition not mirrored in the Sun's less dense core","Gravitational compression affects Earth differently due to its smaller size, leading to a denser composition","Chemical differentiation during Earth's formation resulted in a dense metallic core"},0,"Earth has a higher density than the Sun primarily because it is composed of heavy elements such as iron and nickel, which are denser than the hydrogen and helium that make up most of the Sun."));
    addQuestion(Question("What is the primary source of energy in the Sun?",
                        {"Nuclear fusion of hydrogen into helium", "Gravitational compression", "Radioactive decay of heavy elements", "Heat from the Sun's formation"}, 0, "The primary source of energy in the Sun is nuclear fusion, where hydrogen atoms combine to form helium, releasing energy in the process."));

}
std::string Quiz::getQuestionText(int index) const{
    return questions[index].texte;
}

std::vector<std::string> Quiz::getChoices(int index) const{
    return questions[index].choix;
}

int Quiz::getCorrectAnswer(int index) const{
    return questions[index].bonneReponse;
}

std::string Quiz::getExplanation(int index) const{
    return questions[index].explication;
}

std::string Quiz::getHint(int index) const{
    return questions[index].hint;
}

int Quiz::getQuestionSize() const{
    return questions.size();
}

void Quiz::addScore(int questionTrials){
/*Première tentative correcte : 100% des points.
Deuxième tentative correcte : 50% des points.
Troisième tentative correcte : 25% des points.
Quatrième tentative ou plus : 0% des points.
*/
    if (questionTrials == 1){
        score += 100;
    }
    else if (questionTrials == 2){
        score += 50;
    }
    else if (questionTrials == 3){
        score += 25;
    }
    else{
        score += 0;
    }
}
