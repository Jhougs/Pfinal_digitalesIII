using MongoDB.Bson.Serialization.Attributes;
using MongoDB.Bson;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MqttToDatabase
{
    public class Publisher
    {
        [BsonId]
        [BsonRepresentation(BsonType.ObjectId)]
        public string? Id { get; set; }

        [BsonElement("na")]
        public string? na { get; set; }

        [BsonElement("ty")]
        public DateTime? ty { get; set; }

        
    }
}
