from noise import pnoise2
import numpy as np
from PIL import Image

#map dimensions and noise settings
width = 1024
height = 512
scale = 100.0
octaves = 6
seed = 666

#take in user input for seed
try:
    seed = int(input("Enter a seed value (default is 666): ") or seed)
except ValueError:
    print("Invalid input, using default seed value of 666.") 

def generate_noise_map(seedOffset, octiveOffset=0):
    return np.array([
        [pnoise2((x + seedOffset) / scale, (y + seedOffset) / scale, octaves=octaves+octiveOffset,repeatx=1024, repeaty=1024, base=seed)
                     for x in range(width)] 
                     for y in range(height)
        ])    

def generate_continentalness_map(seedOffset):
    return np.array([
        [pnoise2((x + seedOffset) / scale/2, (y + seedOffset) / scale/2, octaves=4, repeatx=1024, repeaty=1024, base=seedOffset)
                     for x in range(width)] 
                     for y in range(height)
        ])


# Normalize the noise map to a range of 0 to 1
def normalize_noise_map(noise_map):
    min_val = np.min(noise_map)
    max_val = np.max(noise_map)
    return (noise_map - min_val) / (max_val - min_val)

#nomalize the noise map to -1 or 1
def normalize_noise_map_to_neg1_1(noise_map):
    min_val = np.min(noise_map)
    max_val = np.max(noise_map)
    return 2 * (noise_map - min_val) / (max_val - min_val) - 1

#set continentallness map to  -0.5 or 1 based on the noise map
def normalize_continentalness_map(noise_map):
    continentalness_map = np.where(noise_map < 0, -0.5, 1)
    return continentalness_map

#normalize reource map within set values
def normalize_resource_map(resource_map, min_val=0, max_val=1):
    resource_map = np.clip(resource_map, min_val, max_val)
    return (resource_map - min_val) / (max_val - min_val)

def temperature_map(seedOffset):
    equator = height / 2
#generate temperature map based on elevation and moisture and y cordinate so middle of map is warmer
    rawTempNoise =  np.array([
        [pnoise2((x + seedOffset) / scale, (y + seedOffset) / scale, octaves=octaves, repeatx=1024, repeaty=1024, base=seedOffset) 
            + (1 - ((y - equator) / equator) ** 2)*2
                     for x in range(width)] 
                     for y in range(height)
        ])
    #temper = 0.5 * rawTempNoise  +  1 - ((y - equator) / equator) ** 2
    return rawTempNoise


elevation_map = generate_noise_map(seed,100)
moisture_map = generate_noise_map(seed-seed/2*-1,1)
continentalness_map = generate_continentalness_map(seed)
temperature_map = temperature_map(seed)

temperature_map = normalize_noise_map_to_neg1_1(temperature_map)


resourcemap = {}
#resourcemap['iron'] = generate_noise_map(seed)
#resourcemap['coal'] = generate_noise_map(seed)
#resourcemap['gold'] = generate_noise_map(seed)
#resourcemap['copper'] = generate_noise_map(seed)

elevation_map = normalize_noise_map(elevation_map)
moisture_map = normalize_noise_map(moisture_map)
continentalness_map = normalize_noise_map(continentalness_map)

# Normalize the resource maps
#resourcemap['iron'] = normalize_resource_map(resourcemap['iron'])
#resourcemap['coal'] = normalize_resource_map(resourcemap['coal'],0,.5)
#resourcemap['gold'] = normalize_resource_map(resourcemap['gold'],0,.2)
#resourcemap['copper'] = normalize_resource_map(resourcemap['copper'],0,.3)

def classify_biome(elevation,moisture, continentalness,temperature):
    landedness = elevation + continentalness
    if landedness < 0.95:
        if elevation < 0.2:
            return "Deep Ocean"
        elif elevation < 0.7 and moisture < 0.2 and temperature > 0.5:
            return "Ocean" 
        elif elevation > 0.7 and moisture < 0.2 and temperature >= 0.5:
            return "islands"
        elif elevation < 0.2 and moisture < 0.2 and temperature < 0:
            return "arctic"
        else:
            return "Ocean"
        

    elif landedness < 1.0:
        if moisture > 0.5 and temperature > 0.7:
            return "Swamp"
        else:
            return "Beach"
    elif landedness < 1.4:
        if moisture > 0.8 and temperature > 0.7:
            return "Rainforest"
        if moisture > 0.8:
            return "Lake"
        if elevation <0.2 and moisture > 0.8:
            return "Swamp"
        if elevation < 0.2 and moisture < 0.2:
            return "Beach"
        elif moisture > 0.5 and temperature > 0.5:
            return "Temperate Forest"
        elif moisture > 0.2 and temperature > 0.5:
            return "Savanna"
        elif moisture < 0.2 and elevation < 0.5 and temperature > 0.5:
            return "Barren"
        elif moisture < 0.2 and elevation < 0.5 and temperature >0.7:
            return "Desert"
        elif moisture < 0.2 and elevation >= 0.5:
            return "Rocky Desert"
        if moisture > 0.5:
            return "Forest"
        else:
            return "Grassland"
    elif landedness < 1.8:
        if moisture < 0.2 and temperature < 0:
            return "Tundra"
        elif moisture > 0.5 and temperature <0:
            return "Taiga"
        elif moisture < 0.2 and elevation < 0.5:
            return "Barren"
        elif moisture < 0.2 and elevation >= 0.5:
            return "Rocky Barren"
        elif moisture < 0.5 and elevation < 0.5:
            return "Scrubland"
        elif moisture < 0.5 and elevation >= 0.5:
            return "Rocky Scrubland"
        if moisture > 0.5:
            return "Mountain Forest"
        else:
            return "Mountain Grassland"
    elif landedness >= 1.8:
        if moisture < 0.2 and elevation < 0.5:
            return "Rocky Barren"
        elif moisture < 0.2 and elevation >= 0.5:
            return "Mountain"
        elif moisture < 0.5 and elevation < 0.5:
            return "Scrubland"
        elif moisture < 0.5 and elevation >= 0.5:
            return "Rocky Scrubland"
        elif moisture > 0.5 and temperature > 0.7:
            return "Mountain Forest"
        elif moisture > 0.5 and temperature <= 0.7:
            return "Mountain Scrubland"


class WorldFeature:
    # Represents a feature in the world, such as a volcano, oil field, etc.
    #location is a tuple of (x, y) coordinates, used as a minimum distance for the preset type to be placed
    def __init__(self, name, biomes,location,frequency=0.05):
        self.name = name
        self.biomes = biomes
        self.location = location
        self.frequency = frequency
    def __str__(self):
        return f"Feature: {self.name}, Biome: {self.biomes}, Location: {self.location}, Frequency: {self.frequency}"

class WorldFeatureFactory:
    def __init__(self):
        self.features = []
    
    def add_feature(self, name, biome, location, frequency=0.05):
        feature = WorldFeature(name, biome, location, frequency)
        self.features.append(feature)
        return feature
    

    def add_feature(self, feature, location):
        if isinstance(feature, WorldFeature):
            new_feature = WorldFeature(feature.name, feature.biomes, location, feature.frequency)
            #check if feature already exists within minimum distance of of feature location
            closestFeature = self.find_closest_feature_of_same_type(feature.name, location)
            #need to fix location so i can do vector math
            if closestFeature  == None or (closestFeature.location+feature.location < location and closestFeature.location-feature.location > location):
                self.features.append(new_feature)
                return True
        return False

    def __str__(self):
        #create a string of all features
        return "\n".join(str(feature) for feature in self.features)
                    
            
    
    def get_features(self):
        return self.features
    def find_feature_by_biome(self, biome):
        return [feature for feature in self.features if feature.biome == biome]
    def find_closest_feature_of_same_type(self, feature_type, location):
        closest_feature = None
        closest_distance = float('inf')
        for feature in self.features:
            if feature.name == feature_type:
                distance = np.linalg.norm(np.array(feature.location) - np.array(location))
                if distance < closest_distance:
                    closest_distance = distance
                    closest_feature = feature
        return closest_feature


worldFeatures = WorldFeatureFactory()

volcano = WorldFeature("Volcano", "", (0, 0), 0.05)
oilField = WorldFeature("Oil Field", "", (0, 0), 0.05)
meteoriteImpact = WorldFeature("Meteorite Impact", "", (0, 0), 0.05)
mineralDeposit = WorldFeature("Mineral Deposit", "", (0, 0), 0.05)
smugglersPort = WorldFeature("Smugglers Port", "", (0, 0), 0.05)
PeakSummit = WorldFeature("Peak Summit", "", (0, 0), 0.05)
presetFeatures = {"Volcano": volcano, "Oil Field": oilField, "Meteorite Impact": meteoriteImpact, "Mineral Deposit": mineralDeposit, "Smugglers Port": smugglersPort, "Peak Summit": PeakSummit}
#set an area of a biome to be a feature like a volcano, lake, oil field or none etc. 
#create a function that takes in a biome and returns a feature based on the biome and some random chance
import random
def set_feature(biome):
    features = {    
        "Deep Ocean": ["Volcano", "Oil Field"],
        "Ocean": [""],
        "islands": [""],
        "Beach": ["Smugglers Port"],
        "Swamp": ["Oil Field"],
        "Rainforest": ["Meteorite Impact"],
        "Temperate Forest": [""],
        "Savanna": ["Meteorite Impact"],
        "Barren": ["Meteorite Impact", "Oil Field","Mineral Deposit"],
        "Desert": ["Meteorite Impact"],
        "Rocky Desert": ["Canyon", "Meteorite Impact","Mineral Deposit"],
        "Forest": ["Smugglers Port"],
        "Grassland": [""],
        "Tundra": [""],
        "Taiga": [""],
        "Mountain Forest": ["Smugglers Port","Peak Summit"],
        "Mountain Grassland": ["Smugglers Port","Peak Summit"],
        "Rocky Barren": ["Smugglers Port", "Mineral Deposit", "Volcano"],
        "Rocky Scrubland": [""],
        "Scrubland": [""],
        "Mountain Scrubland": [""],
        "Mountain": ["Peak Summit"],
        "Lake": [""]
        
    }
    return random.choice(features.get(biome,[]) )  
#color map based on biome classification
def color_map(biome):
    colors = {
        "Deep Ocean": (0, 0, 128),
        "Ocean": (0, 0, 255),
        "islands": (255, 255, 0),
        "Beach": (255, 202, 110),
        "Swamp": (45, 125, 96),
        "Rainforest": (0, 128, 0),
        "Temperate Forest": (34, 139, 34),
        "Savanna": (210, 180, 140),
        "Barren": (139, 69, 19),
        "Desert": (204, 151, 59),
        "Rocky Desert": (205, 133, 63),
        "Forest": (34, 139, 34),
        "Grassland": (64, 207, 62),
        "Tundra": (255, 250, 205),
        "Taiga": (100, 100, 125),
        "Mountain Forest": (34, 200, 34),
        "Mountain Grassland": (124, 252, 0),
        "Rocky Barren": (139, 137, 137),
        "Rocky Scrubland": (139, 137, 137),
        "Scrubland": (16, 138, 3),
        "Mountain Scrubland": (128, 178, 128),
        "Mountain": (255, 255, 255),
        "Lake": (0, 191, 255),
        "arctic": (240, 248, 255),
    }
    return colors.get(biome)


rgb_data = np.zeros((height, width, 3), dtype=np.uint8)
for y in range(height):
    for x in range(width):
        elevationpoint = elevation_map[y, x]
        moisturepoint = moisture_map[y, x]
        continentalnesspoint = continentalness_map[y, x]
        temperaturepoint = temperature_map[y, x]
        #biome = classify_biome(elevation_map[y, x], moisture_map[y, x], continentalness_map[y, x], temperature_map[y, x])
        biome = classify_biome(elevationpoint, moisturepoint, continentalnesspoint, temperaturepoint)

        #if biome is None, set it to Ocean
        if biome is None:
            biome = "Ocean"

        
        rgb_data[y, x] = color_map(biome)
        #set a feature based on the biome
        feature = presetFeatures.get(set_feature(biome))
        if feature and feature != "" and feature.frequency >= random.random():
            #add the feature to the worldFeatures factory
            if worldFeatures.add_feature(feature, (x, y)):
                rgb_data[y, x] = (255, 0, 0)  # Mark feature locations in red
            #print(f"Added feature {feature} at ({x}, {y}) in biome {biome}")
            

img = Image.fromarray(rgb_data, 'RGB')

#save image with seed in filename
img.save(f"world_map_seed_{seed}.png")

temp_img = Image.fromarray((temperature_map + 1) * 127.5).convert("L")
temp_img.save(f"temperature_map_seed_{seed}.png")
#save elevation map
elevation_img = Image.fromarray((elevation_map * 255).astype(np.uint8), 'L')
elevation_img.save(f"elevation_map_seed_{seed}.png")
#save moisture map
moisture_img = Image.fromarray((moisture_map * 255).astype(np.uint8), 'L')
moisture_img.save(f"moisture_map_seed_{seed}.png")
#save continentalness map
continentalness_img = Image.fromarray((continentalness_map * 255).astype(np.uint8
), 'L')
continentalness_img.save(f"continentalness_map_seed_{seed}.png")   
#save resource maps

print(worldFeatures)