#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unordered_map>
#include <deque>


/*
Для проверки
4
BOOK 1 hotel1 1 1
BOOK 2 hotel1 2 2
CLIENTS hotel1
ROOMS hotel1

*/


struct Booking {
  int64_t booking_time;
  uint client_id;
  uint rooms_count;
};

class Hotel {
private:
  std::unordered_map< uint, uint> clients_;
  uint rooms_booking_count_;
  std::deque<Booking> bookings_;
public:
  size_t getClientsCount() {
    return clients_.size();
  }

  size_t getRoomsCount() {
    return rooms_booking_count_;
  }

  void book( const Booking& booking ) {

    rooms_booking_count_ += booking.rooms_count;
    clients_[booking.client_id] += booking.rooms_count;

    printf("Client size: %ld\n", clients_.size());

  }
};

class BookingManager {
private:
  int64_t last_time_; //Время последнего события
  std::unordered_map<std::string, Hotel> hotels_;
public:

  BookingManager():
    last_time_(0)
{}

  void bookHotel( int64_t book_time, const std::string& hotel_name, unsigned int client_id, unsigned int rooms_count ) {
    last_time_ = book_time;
    hotels_[hotel_name].book( {book_time, client_id, rooms_count} );
  }

  size_t getHotelClientsCount( const std::string &hotel_name ) {
      return hotels_[hotel_name].getClientsCount();
  }


  size_t getHotelRoomsCount( const std::string &hotel_name ) {
      return hotels_[hotel_name].getRoomsCount();
  }

};


int main() {

  BookingManager booking_manager;

  int q;
  std::cin >> q;
  for (int i=0; i < q; ++i ) {
    printf("Шаг %d: введите команду\n", i );

    std::string cmd_type = "";
    std::cin >> cmd_type;

    if ( cmd_type == "BOOK" ) {
      int64_t book_time;
      std::string hotel_name ="";
      uint client_id, rooms_count;
      std::cin >> book_time >> hotel_name >> client_id >> rooms_count;
      booking_manager.bookHotel( book_time, hotel_name, client_id, rooms_count  );


    } else if ( cmd_type == "CLIENTS" ) {
      std::string hotel_name = "";
      std::cin >>  hotel_name;
      std::cout << booking_manager.getHotelClientsCount( hotel_name ) << std::endl;

    } else if ( cmd_type == "ROOMS") {
      std::string hotel_name = "";
      std::cin >> hotel_name;
      std::cout << booking_manager.getHotelRoomsCount( hotel_name ) << std::endl;

    } else {
      abort();
    }



  }
  return 0;
}
