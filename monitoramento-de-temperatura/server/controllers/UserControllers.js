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

  getAllUsers = async (req, res) => {
    try {
      const users = await this.userService.getAllUsers();
      res.status(200).json(users);
    } catch (error) {
      console.error("Error fetching users:", error);
      res.status(400).json({ error: error.message });
    }
  }

  updateUser = async (req, res) => {
    try {
      const { userId } = req.params;
      const updateData = req.body;
      const updatedUser = await this.userService.updateUser(userId, updateData);
      res.status(200).json(updatedUser);
    } catch (error) {
      console.error("Error updating user:", error);
      res.status(400).json({ error: error.message });
    }
  }

  deleteUser = async (req, res) => {
    try {
      const { userId } = req.params;
      await this.userService.deleteUser(userId);
      res.status(204).send();
    } catch (error) {
      console.error("Error deleting user:", error);
      res.status(400).json({ error: error.message });
    }
  }
}

export default UserController;