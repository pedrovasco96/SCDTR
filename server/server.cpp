  #include <iostream>
  #include <boost/bind.hpp>
  #include <boost/enable_shared_from_this.hpp>
  #include <boost/asio.hpp>
  #include <boost/shared_ptr.hpp>
  #include <thread>

  using namespace boost; 
  using namespace boost::asio; 
  using ip::tcp;

  class conn :  public enable_shared_from_this<conn> {
  private:  
      boost::asio::streambuf sock_buff;
     	tcp::socket sock_;
     	std::string msg_;
      std::string sub_msg_;
      std::ostringstream os;
      serial_port sp;
      size_t str_pos;
     	conn(io_service& io) :  sock_(io), sp(io)  {
       sp.open("/dev/ttyACM0");    
       sp.set_option(serial_port_base::baud_rate(115200));
      }
     	void req_acceptor()   {
     		
        boost::asio::async_read_until(sock_, sock_buff, '\n', boost::bind(&conn::req_handler, shared_from_this()));

     	}

      void req_handler(){
        std::istream is(&sock_buff);
        std::getline(is, msg_);

        if(!msg_.empty()){
          std::cout << "Request: " << msg_ << std::endl;
          if(msg_[0] == 's'){
            str_pos = msg_.find_last_of(" ");
            sub_msg_ = msg_.substr(2, str_pos-2);
            std::cout << "arduino: " << sub_msg_ << std::endl;
            os << sub_msg_;
            
            async_write(sp, buffer(os.str()), boost::bind(&conn::ack_handler, shared_from_this()));

          }
          else if(msg_[0] == 'r'){
            os << msg_[0];
            
            async_write(sp, buffer(os.str()), boost::bind(&conn::ack_handler, shared_from_this()));

          }


        }
        else{
          req_acceptor();
        }
      }
      void ack_handler(){
        boost::asio::async_write(sock_, buffer("ack\n"), boost::bind(&conn::req_acceptor, shared_from_this()));
      }

  public: 
  	static shared_ptr<conn> create(io_service& io) {
           return shared_ptr<conn>(new conn(io));
      }
      tcp::socket& socket() {return sock_;}
      void start() {
        std::cout << "Connection established" << std::endl;
      	async_write(sock_,buffer("Connection established\n"),
      		boost::bind(&conn::req_acceptor, shared_from_this())); 
      }
  };
  class tcp_server {
  private:  
      tcp::acceptor acceptor_;
  public:  
      tcp_server(io_service& io)
       : acceptor_(io, tcp::endpoint(tcp::v4(), 10000))  {
                     start_accept();
       }
  private:  
     void start_accept() {
         shared_ptr<conn> new_conn =
              conn::create(acceptor_.get_io_service());
         acceptor_.async_accept(new_conn->socket(),
         boost::bind(&tcp_server::h_accept, this, new_conn));
     }
    void h_accept(shared_ptr<conn> new_conn)  {
           new_conn->start();
           start_accept();
     }
  };

  class sniff_class{
    private:

    public:
      sniff_class(io_service &io): {

      }

    private:

  }

  int main()  {
      io_service io;
      io_service sniffer;

      tcp_server server(io);
      io.run();

      std::threah thr_sniff {[&sniffer](){
        sniff_class serial(sniffer);
        sniffer.run();
      }

  }

