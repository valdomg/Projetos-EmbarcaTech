import User from "../models/User.js";
import UserService from "../services/User/UserServices.js";

class UserController {
  constructor() {
    this.userService = new UserService(User);
  }

  createUser = async (req, res) => {
    try {
      const userData = req.body;
      const user = await this.userService.createUser(userData);
      res.status(201).json(user);
    } catch (error) {
      console.error("Error creating user:", error);
      res.status(400).json({ error: error.message });
    }
  }

  getUserByEmail = async (req, res) => {
    try {
      const { email } = req.params;
      const user = await this.userService.getUserByEmail(email);
      if (!user) {
        return res.status(404).json({ error: "User not found" });
      }
      res.status(200).json(user);
    } catch (error) {
      console.error("Error fetching user:", error);
      res.status(400).json({ error: error.message });
    }
  };

}

export default UserController;