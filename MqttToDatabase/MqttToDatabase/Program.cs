using MongoDB.Bson;
using MongoDB.Driver;
using MQTTnet;
using MQTTnet.Client;
using MQTTnet.Client.Options;
using System;
using System.Text;
using System.Threading.Tasks;


namespace MqttToDatabase
{
    class Program
    {
        static async Task  Main(string[] args)
        {
            while (true)
            {
                var mqttFactory = new MqttFactory();
                IMqttClient client = mqttFactory.CreateMqttClient();
                var options = new MqttClientOptionsBuilder().WithClientId(Guid.NewGuid().ToString()).WithTcpServer("31.220.107.127", 1883).WithCredentials("sensores", "Juanviana10_")
                    .WithCleanSession().Build();

                client.UseConnectedHandler( e => {
                    Console.WriteLine("Connected to broker");
                    var topicFilter = new MqttTopicFilterBuilder().WithTopic("frecuencia").Build();
                    client.SubscribeAsync(topicFilter).Wait();
                    client.UseApplicationMessageReceivedHandler(e =>
                    {
                        Console.WriteLine($"received message - {Encoding.UTF8.GetString(e.ApplicationMessage.Payload)}");
                        var valorLeido = Encoding.UTF8.GetString(e.ApplicationMessage.Payload);
                        sendToDatabse(valorLeido);
                    });

                    
                });
                client.UseDisconnectedHandler(e => {
                    Console.WriteLine("No se conectó ");
                });

                

                await client.ConnectAsync(options);
                Console.ReadLine();



                await client.DisconnectAsync();

            }
            
            
            


        }

        private static void sendToDatabse(string valorLeido)
        {
            Console.WriteLine("ingresando a escritura");
            string direccion =   "mongodb+srv://desarrollohmv:06dmhr7fKNYJOdeX@i2msprueba.v7ihp.mongodb.net/myFirstDatabase?retryWrites=true&w=majority"; //"mongodb://adminiot:oGnwdKSSwc2sN53x@cluster0-shard-00-00.6lvdx.mongodb.net:27017,cluster0-shard-00-01.6lvdx.mongodb.net:27017,cluster0-shard-00-02.6lvdx.mongodb.net:27017/myFirstDatabase?ssl=true&replicaSet=atlas-fhrtnc-shard-0&authSource=admin&retryWrites=true&w=majority";//"mongodb+srv://adminiot:oGnwdKSSwc2sN53x@cluster0.6lvdx.mongodb.net/myFirstDatabase?retryWrites=true&w=majority;";         
            //string direccion = "mongodb+srv://desarrollohmv:06dmhr7fKNYJOdeX@i2msprueba.v7ihp.mongodb.net/test?authSource=admin&replicaSet=atlas-ovjmqh-shard-0&readPreference=primary&ssl=true";

            var mongoClient = new MongoClient(direccion);
            var database = mongoClient.GetDatabase("sensores");
            var collection = database.GetCollection<YourClass>("frecuencia");
           

            var document = new YourClass
                {

                Key1= valorLeido ,
                Timestamp= DateTime.Now ,
            // Add more fields as needed
                };
            //var conversion = float.Parse(valorLeido);
           
            Console.WriteLine("conectado a databse");
            collection.InsertOne(document);
            Console.WriteLine("conectado a databse;");

            Console.WriteLine("Documento isertador con exito");
        }


    }

    public class YourClass
    {
        public string Key1 { get; set; }
        public DateTime Timestamp { get; set; }
        // Add more properties as needed
    }
}
