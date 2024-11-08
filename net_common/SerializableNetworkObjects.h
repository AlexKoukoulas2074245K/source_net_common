///------------------------------------------------------------------------------------------------
///  SerializableNetworkObjects.h
///  TinyMMOCommon
///                                                                                                
///  Created by Alex Koukoulas on 30/04/2024
///------------------------------------------------------------------------------------------------

#ifndef SerializableNetworkObjects_h
#define SerializableNetworkObjects_h

///------------------------------------------------------------------------------------------------

#if __has_include(<engine/utils/StringUtils.h>)
#include <engine/utils/StringUtils.h>
#else
#include "../util/StringUtils.h"
#endif

#if __has_include(<engine/utils/MathUtils.h>)
#include <engine/utils/MathUtils.h>
#else
#include "../util/MathUtils.h"
#endif

#if __has_include(<nlohmann/json.hpp>)
#include <nlohmann/json.hpp>
#else
#include "../util/Json.h"
#endif

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
inline std::string toString(const std::vector<std::string>& v) { return strutils::VecToString(v); }
inline std::string toString(const std::vector<long long>& v) { return strutils::VecToString(v); }

inline void addToJson(nlohmann::json& json, const std::string& fieldName, const strutils::StringId& s) { json[fieldName] = s.GetString(); }
inline void addToJson(nlohmann::json& json, const std::string& fieldName, const std::string& s) { json[fieldName] = s; }
inline void addToJson(nlohmann::json& json, const std::string& fieldName, const float f) { json[fieldName] = f; }
inline void addToJson(nlohmann::json& json, const std::string& fieldName, const bool b) { json[fieldName] = b; }
inline void addToJson(nlohmann::json& json, const std::string& fieldName, const int i) { json[fieldName] = i; }
inline void addToJson(nlohmann::json& json, const std::string& fieldName, const long long l) { json[fieldName] = l; }
inline void addToJson(nlohmann::json& json, const std::string& fieldName, const glm::vec3& v) { json[fieldName]["x"] = v.x; json[fieldName]["y"] = v.y; json[fieldName]["z"] = v.z; }
inline void addToJson(nlohmann::json& json, const std::string& fieldName, const std::vector<std::string>& v) { json[fieldName] = v; }
inline void addToJson(nlohmann::json& json, const std::string& fieldName, const std::vector<long long>& v) { json[fieldName] = v; }

inline void parseValue(const nlohmann::json& json, const std::string& fieldName, strutils::StringId& fieldValue) { fieldValue = strutils::StringId(json[fieldName].get<std::string>()); }
inline void parseValue(const nlohmann::json& json, const std::string& fieldName, std::string& fieldValue) { fieldValue = json[fieldName].get<std::string>(); }
inline void parseValue(const nlohmann::json& json, const std::string& fieldName, float& fieldValue) { fieldValue = json[fieldName].get<float>(); }
inline void parseValue(const nlohmann::json& json, const std::string& fieldName, bool& fieldValue) { fieldValue = json[fieldName].get<bool>(); }
inline void parseValue(const nlohmann::json& json, const std::string& fieldName, int& fieldValue) { fieldValue = json[fieldName].get<int>(); }
inline void parseValue(const nlohmann::json& json, const std::string& fieldName, long long& fieldValue) { fieldValue = json[fieldName].get<long long>(); }
inline void parseValue(const nlohmann::json& json, const std::string& fieldName, glm::vec3& fieldValue) { if (json[fieldName].count("x") && json[fieldName].count("y") && json[fieldName].count("z")) fieldValue = glm::vec3(json[fieldName]["x"].get<float>(), json[fieldName]["y"].get<float>(), json[fieldName]["z"].get<float>()); }
inline void parseValue(const nlohmann::json& json, const std::string& fieldName, std::vector<std::string>& fieldValue) { fieldValue = json[fieldName].get<std::vector<std::string>>(); }
inline void parseValue(const nlohmann::json& json, const std::string& fieldName, std::vector<long long>& fieldValue) { fieldValue = json[fieldName].get<std::vector<long long>>(); }

// Construct class outline
#define BEGIN_SERIALIZABLE(className) struct className { \
inline static const char* ObjectCollectionHeader() { return #className; } \
inline std::string SerializeToJsonString() const; \
inline nlohmann::json SerializeToJson() const; \
inline void DeserializeFromJson(const nlohmann::json& json);
#define VALUE(typeName, fieldName)     typeName fieldName;
#define END_SERIALIZABLE() };

#include "NetworkObjectDefinitions.inc"

#undef BEGIN_SERIALIZABLE
#undef VALUE
#undef END_SERIALIZABLE

// Define serialization to string method
#define BEGIN_SERIALIZABLE(className) std::string className::SerializeToJsonString() const {std::stringstream s; s << "{";
#define VALUE(typeName, fieldName)     s << "\"" << #fieldName << "\": " << toString(fieldName) << ",";
#define END_SERIALIZABLE() s << "\"_\": 0}"; return s.str(); }

#include "NetworkObjectDefinitions.inc"

#undef BEGIN_SERIALIZABLE
#undef VALUE
#undef END_SERIALIZABLE

// Define serialization to json method
#define BEGIN_SERIALIZABLE(className) nlohmann::json className::SerializeToJson() const {nlohmann::json json;
#define VALUE(typeName, fieldName)  addToJson(json, #fieldName, fieldName);
#define END_SERIALIZABLE() return json; }

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
