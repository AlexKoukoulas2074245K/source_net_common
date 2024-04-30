///------------------------------------------------------------------------------------------------
///  SerializableNetworkObjects.h
///  TinyMMOCommon
///                                                                                                
///  Created by Alex Koukoulas on 30/04/2024
///------------------------------------------------------------------------------------------------

#ifndef SerializableNetworkObjects_h
#define SerializableNetworkObjects_h

///------------------------------------------------------------------------------------------------

#include <engine/utils/StringUtils.h>
#include <engine/utils/MathUtils.h>
#include <nlohmann/json.hpp>
#include <sstream>

///------------------------------------------------------------------------------------------------

namespace networking
{

///------------------------------------------------------------------------------------------------

inline std::string toString(const strutils::StringId& s) { return "\"" + s.GetString() + "\""; }
inline std::string toString(const std::string& s) { return "\"" + s + "\""; }
inline std::string toString(const float f) { return std::to_string(f); }
inline std::string toString(const bool b) { return b ? "true" : "false"; }
inline std::string toString(const int i) { return std::to_string(i); }
inline std::string toString(const long long l) { return std::to_string(l); }
inline std::string toString(const glm::vec3& v) { return "{\"x\": " + std::to_string(v.x) + ", \"y\": " + std::to_string(v.y) + ", \"z\": " + std::to_string(v.z) + "}"; }

inline void parseValue(const nlohmann::json& json, const std::string& fieldName, strutils::StringId& fieldValue) { fieldValue = strutils::StringId(json[fieldName].get<std::string>()); }
inline void parseValue(const nlohmann::json& json, const std::string& fieldName, std::string& fieldValue) { fieldValue = json[fieldName].get<std::string>(); }
inline void parseValue(const nlohmann::json& json, const std::string& fieldName, float& fieldValue) { fieldValue = json[fieldName].get<float>(); }
inline void parseValue(const nlohmann::json& json, const std::string& fieldName, bool& fieldValue) { fieldValue = json[fieldName].get<bool>(); }
inline void parseValue(const nlohmann::json& json, const std::string& fieldName, int& fieldValue) { fieldValue = json[fieldName].get<int>(); }
inline void parseValue(const nlohmann::json& json, const std::string& fieldName, long long& fieldValue) { fieldValue = json[fieldName].get<long long>(); }
inline void parseValue(const nlohmann::json& json, const std::string& fieldName, glm::vec3& fieldValue) { if (json[fieldName].count("x") && json[fieldName].count("y") && json[fieldName].count("z")) fieldValue = glm::vec3(json[fieldName]["x"].get<float>(), json[fieldName]["y"].get<float>(), json[fieldName]["z"].get<float>()); }

// Construct class outline
#define BEGIN_SERIALIZABLE(className) struct className { \
inline std::string SerializeToJsonString() const; \
inline void DeserializeFromJson(const nlohmann::json& json);
#define VALUE(typeName, fieldName)     typeName fieldName;
#define END_SERIALIZABLE() };

#include "NetworkObjectDefinitions.inc"

#undef BEGIN_SERIALIZABLE
#undef VALUE
#undef END_SERIALIZABLE

// Define serialization method
#define BEGIN_SERIALIZABLE(className) std::string className::SerializeToJsonString() const {std::stringstream s; s << "{";
#define VALUE(typeName, fieldName)     s << "\"" << #fieldName << "\": " << toString(fieldName) << ",";
#define END_SERIALIZABLE() s << "\"_\": 0}"; return s.str(); }

#include "NetworkObjectDefinitions.inc"

#undef BEGIN_SERIALIZABLE
#undef VALUE
#undef END_SERIALIZABLE

// Define deserialization method
#define BEGIN_SERIALIZABLE(className) void className::DeserializeFromJson(const nlohmann::json& json) {
#define VALUE(typeName, fieldName) if(json.count(#fieldName)) parseValue(json, #fieldName, fieldName);
#define END_SERIALIZABLE() }

#include "NetworkObjectDefinitions.inc"

#undef BEGIN_SERIALIZABLE
#undef VALUE
#undef END_SERIALIZABLE

///-----------------------------------------------------------------------------------------------

}

///------------------------------------------------------------------------------------------------

#endif /* SerializableNetworkObjects_h */
